/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "x/codegen/CheckFailureSnippet.hpp"

#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Relocation.hpp"
#include "codegen/SnippetGCMap.hpp"
#include "env/IO.hpp"
#include "env/jittypes.h"
#include "env/VMJ9.h"
#include "il/LabelSymbol.hpp"
#include "il/MethodSymbol.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/RegisterMappedSymbol.hpp"
#include "il/ResolvedMethodSymbol.hpp"
#include "il/StaticSymbol.hpp"
#include "il/Symbol.hpp"
#include "infra/SimpleRegex.hpp"
#include "objectfmt/GlobalFunctionCallData.hpp"
#include "objectfmt/ObjectFormat.hpp"
#include "runtime/CodeCacheManager.hpp"

uint32_t TR::X86CheckFailureSnippet::getLength(int32_t estimatedSnippetStart)
   {
   uint32_t breakSize = 0;
   if(_breakOnThrowType)
   {
      switch (getDestination()->getReferenceNumber())
      {
         case TR_nullCheck:
         {
            if (_breakOnThrowType&TR_BREAKONTHROW_NPE)
            {
               breakSize=1;
            }
            break;
         }
         case TR_arrayBoundsCheck:
         {
            if (_breakOnThrowType&TR_BREAKONTHROW_AIOB)
            {
               breakSize=1;
            }
            break;
         }
      }

   }
   return (_requiresFPstackPop ? 2 :   // FSTP st0,st0
                                 0)
           + cg()->getObjFmt()->globalFunctionCallBinaryLength()
           + 4                         // disp32 to check instruction
           + breakSize;
   }

uint8_t *TR::X86CheckFailureSnippet::emitSnippetBody()
   {
   uint8_t *buffer = cg()->getBinaryBufferCursor();
   getSnippetLabel()->setCodeLocation(buffer);
   buffer = emitCheckFailureSnippetBody(buffer);
   return buffer;
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::X86CheckFailureSnippet * snippet)
   {
   if (pOutFile == NULL)
      return;

   TR::SymbolReference *symRef    = snippet->getDestination();
   TR::MethodSymbol    *sym       = symRef->getSymbol()->castToMethodSymbol();
   uint8_t            *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet), getName(symRef));

   if (snippet->getRequiredFPstackPop())
      {
      printPrefix(pOutFile, NULL, bufferPos, 2);
      trfprintf(pOutFile, "fstp\tst(0)\t\t%s Discard top of FP stack",
                    commentString());
      bufferPos += 2;
      }

   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile, "call\t%s \t\t%s Helper Address = " POINTER_PRINTF_FORMAT,
                 getName(symRef),
                 commentString(),
                 sym->getMethodAddress());
   bufferPos += 5;

   printPrefix(pOutFile, NULL, bufferPos, 4);
   trfprintf(pOutFile,
                 "%s \t%s%08x%s",
                 ddString(),
                 hexPrefixString(),
                 bufferPos - snippet->getCheckInstruction()->getBinaryEncoding(),
                 hexSuffixString());
   }


uint8_t *TR::X86CheckFailureSnippet::emitCheckFailureSnippetBody(uint8_t *buffer)
   {
   TR_J9VMBase *fej9 = (TR_J9VMBase *)(cg()->fe());

   // add int3 instruction if breakOnThrow is set on NPE or AIOB
   if(_breakOnThrowType)
   {
      switch (getDestination()->getReferenceNumber())
      {
         case TR_nullCheck:
         {
            if (_breakOnThrowType&TR_BREAKONTHROW_NPE)
            {
               *buffer++ = 0xcc;
            }
            break;
         }
         case TR_arrayBoundsCheck:
         {
            if (_breakOnThrowType&TR_BREAKONTHROW_AIOB)
            {
               *buffer++ = 0xcc;
            }
            break;
         }
      }
   }
   // Discard the top of the FP stack
   //
   if (_requiresFPstackPop)
      {
      *buffer++ = 0xdd;  // FSTP st0,st0
      *buffer++ = 0xd8;
      }

   TR::GlobalFunctionCallData data(getDestination(), getCheckInstruction()->getNode(), buffer, cg());
   buffer = cg()->getObjFmt()->encodeGlobalFunctionCall(data);

   uint8_t *checkSite = getCheckInstruction()->getBinaryEncoding();
   *(uint32_t *)buffer = (uint32_t)(buffer - checkSite);
   buffer += 4;

   // Up until this point we haven't marked the jump instruction getCheckInstruction() as being a gc safe
   // point and we've marked the snippet as being a gc point.  This call to registerStackMap is going add
   // the gc map to the atlas and associate it with the jump instruction.  The flags on the instruction
   // and snippet are set back to reflect this.
   //
   // The real fix is to change the helper and snippet to not do the return code adjust, have the return
   // address be the snippet and associate the gc map with the snippet.
   //
   gcMap().registerStackMap(checkSite, cg());
   if (gcMap().getStackMap())
      {
      getCheckInstruction()->setNeedsGCMap();
      gcMap().resetGCSafePoint();
      resetNeedsExceptionTableEntry();
      }

   return buffer;
   }


void TR::X86CheckFailureSnippet::checkBreakOnThrowOption()
   {
   TR::SimpleRegex *r = cg()->comp()->getOptions()->getBreakOnThrow();
   if (r && (TR::SimpleRegex::matchIgnoringLocale(r, "java/lang/NullPointerException")
                || TR::SimpleRegex::matchIgnoringLocale(r, "NPE", false)))
      {
      _breakOnThrowType|=TR_BREAKONTHROW_NPE;
      }
   if (r && (TR::SimpleRegex::matchIgnoringLocale(r, "java/lang/ArrayIndexOutOfBoundsException")
               || TR::SimpleRegex::matchIgnoringLocale(r, "AIOB", false)))
      {
      _breakOnThrowType|=TR_BREAKONTHROW_AIOB;
      }
   }


uint32_t TR::X86BoundCheckWithSpineCheckSnippet::getLength(int32_t estimatedSnippetStart)
   {
   return 0;
   }

uint8_t *TR::X86BoundCheckWithSpineCheckSnippet::emitSnippetBody()
   {
   uint8_t *buffer = cg()->getBinaryBufferCursor();
   getSnippetLabel()->setCodeLocation(buffer);
   return buffer;
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86BoundCheckWithSpineCheckSnippet *snippet)
   {
   if (pOutFile == NULL)
      return;

   TR::SymbolReference *symRef    = snippet->getDestination();
   TR::MethodSymbol    *sym       = symRef->getSymbol()->castToMethodSymbol();
   uint8_t            *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet), getName(symRef));

   trfprintf(pOutFile, "\t\t\t\t\t\t\t\t\t%s bound check with spine check snippet", commentString());

   }

uint32_t TR::X86SpineCheckSnippet::getLength(int32_t estimatedSnippetStart)
   {
   return 0;
   }

uint8_t *TR::X86SpineCheckSnippet::emitSnippetBody()
   {
   uint8_t *buffer = cg()->getBinaryBufferCursor();
   getSnippetLabel()->setCodeLocation(buffer);
   return buffer;
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86SpineCheckSnippet *snippet)
   {
   if (pOutFile == NULL)
      return;

   TR::SymbolReference *symRef    = snippet->getDestination();
   TR::MethodSymbol    *sym       = symRef->getSymbol()->castToMethodSymbol();
   uint8_t            *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet), getName(symRef));

   trfprintf(pOutFile, "\t\t\t\t\t\t\t\t\t%s spine check snippet", commentString());

   }


uint32_t TR::X86CheckFailureSnippetWithResolve::getLength(int32_t estimatedSnippetStart)
   {
   return (getRequiredFPstackPop() ? 2 :  // FSTP st0, st0
                                     0)
           + 5 + 5 + 5                    // 3 * PUSHImm4
           + (2 * cg()->getObjFmt()->globalFunctionCallBinaryLength())  // 2 * global function calls
           + 4;                           // disp32 to check instruction
   }

enum
   {
   resolveHasLiveXMMRs = 0x10000000,
   longPushTag         = 0x00800000,
   dontPatchTag        = 0x00400000
   };

uint8_t *TR::X86CheckFailureSnippetWithResolve::emitSnippetBody()
   {
   TR_J9VMBase *fej9 = (TR_J9VMBase *)(cg()->fe());
   uint8_t *buffer = cg()->getBinaryBufferCursor();
   getSnippetLabel()->setCodeLocation(buffer);

   *buffer++ = 0x68; // push   imm4 (return address is the throw of the nullpointer)
   *(int32_t *)buffer = (int32_t) (intptr_t) (buffer + 24);
   buffer += 4;

   int32_t cpIndexValue = getDataSymbolReference()->getCPIndex();

   *buffer++ = 0x68; // push   imm4

   // Tag the value so that we can recognise the long case
   //
   if (getHasLiveXMMRs())
         cpIndexValue |= resolveHasLiveXMMRs;

   *(int32_t *)buffer = (getNumLiveX87Registers() << 24) | dontPatchTag | longPushTag | cpIndexValue;
   buffer += 4;


   // Push address of literal pool
   //
   *buffer++ = 0x68; // push   imm4
   *(int32_t *)buffer = (int32_t) (intptr_t) getDataSymbolReference()->getOwningMethod(cg()->comp())->constantPool();
   cg()->addExternalRelocation(new (cg()->trHeapMemory()) TR::ExternalRelocation(buffer,
                                                                           *(uint8_t **)buffer,
                                                                           getCheckInstruction()->getNode() ? (uint8_t *)(uintptr_t)getCheckInstruction()->getNode()->getInlinedSiteIndex() : (uint8_t *)-1,
                                                                           TR_ConstantPool, cg()),
                          __FILE__, __LINE__,
                          getCheckInstruction()->getNode());
   buffer += 4;

   TR::SymbolReference * glueSymRef = cg()->symRefTab()->findOrCreateRuntimeHelper(getHelper());
   TR::GlobalFunctionCallData dataGlue(glueSymRef, getCheckInstruction()->getNode(), buffer, cg());
   buffer = cg()->getObjFmt()->encodeGlobalFunctionCall(dataGlue);

   // Discard the top of the FP stack
   //
   if (getRequiredFPstackPop())
      {
      *buffer++ = 0xdd;  // FSTP st0,st0
      *buffer++ = 0xd8;
      }

   TR::GlobalFunctionCallData dataDestination(getDestination(), getCheckInstruction()->getNode(), buffer, cg());
   buffer = cg()->getObjFmt()->encodeGlobalFunctionCall(dataDestination);

   uint8_t *checkSite = getCheckInstruction()->getBinaryEncoding();
   *(uint32_t *)buffer = (uint32_t)(buffer - checkSite);
   buffer += 4;

   // Up until this point we haven't marked the jump instruction getCheckInstruction() as being a gc safe point and
   // we've marked the snippet as being a gc point.  This call to registerStackMap is going add the gc map to the atlas
   // and associate it with the jump instruction.  The flags on the instruction and snippet are set back to reflect this.
   // The real fix is to change the helper and snippet to not do the return code adjust,  have the return address be the
   // snippet and associate the gc map with the snippet.
   //
   gcMap().registerStackMap(checkSite, cg());
   if (gcMap().getStackMap())
      {
      getCheckInstruction()->setNeedsGCMap();
      gcMap().resetGCSafePoint();
      resetNeedsExceptionTableEntry();
      }

   return buffer;
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::X86CheckFailureSnippetWithResolve * snippet)
   {

   enum
      {
      longPushTag  = 0x00800000,
      dontPatchTag = 0x00400000
      };

   if (pOutFile == NULL)
      return;
   TR::SymbolReference *symRef    = snippet->getDestination();
   TR::MethodSymbol    *sym       = symRef->getSymbol()->castToMethodSymbol();
   uint8_t            *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet), getName(symRef));

   TR::SymbolReference *methodSymRef = snippet->getNode()->getSymbolReference();
   TR::MethodSymbol    *methodSymbol = methodSymRef->getSymbol()->castToMethodSymbol();
   int32_t cpIndexValue = ((TR::X86CheckFailureSnippetWithResolve *)snippet)->getDataSymbolReference()->getCPIndex();
   int32_t size = 5;

   printPrefix(pOutFile, NULL, bufferPos, size);
   trfprintf(pOutFile, "push\t" POINTER_PRINTF_FORMAT "\t\t%s push return address which is the throw bellow", bufferPos+24,
                 commentString());
   bufferPos += 5;

   cpIndexValue |= dontPatchTag;
   cpIndexValue |= longPushTag;
   printPrefix(pOutFile, NULL, bufferPos, size);
   trfprintf(pOutFile, "push\t" POINTER_PRINTF_FORMAT "\t\t%s push cpIndex", cpIndexValue,
                 commentString());
   bufferPos += size;

   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile,
           "push\t" POINTER_PRINTF_FORMAT "\t\t%s push address of constant pool",
                 getOwningMethod(methodSymRef)->constantPool(),
                 commentString());
   bufferPos += 5;

   printPrefix(pOutFile, NULL, bufferPos, 5);

   trfprintf(pOutFile, "call\tResolve Function For the Child to the NULLChk");
   bufferPos += 5;

   if (snippet->getRequiredFPstackPop())
      {
      printPrefix(pOutFile, NULL, bufferPos, 2);
      trfprintf(pOutFile, "fstp\tst(0)\t\t%s Discard top of FP stack",
                    commentString());
      bufferPos += 2;
      }

   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile, "call\t%s \t\t%s Helper Address = " POINTER_PRINTF_FORMAT,
                 getName(symRef),
                 commentString(),
                 sym->getMethodAddress());
   bufferPos += 5;

   printPrefix(pOutFile, NULL, bufferPos, 4);
   trfprintf(pOutFile,
                 "%s \t%s%08x%s",
                 ddString(),
                 hexPrefixString(),
                 bufferPos - snippet->getCheckInstruction()->getBinaryEncoding(),
                 hexSuffixString());
   }
