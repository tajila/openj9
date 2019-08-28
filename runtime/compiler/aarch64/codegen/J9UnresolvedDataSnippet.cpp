/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
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

#include "codegen/CodeGenerator.hpp"
#include "codegen/UnresolvedDataSnippet.hpp"
#include "compile/Compilation.hpp"
#include "il/symbol/StaticSymbol.hpp"

uint8_t *
J9::ARM64::UnresolvedDataSnippet::emitSnippetBody()
   {
   // Implement this in OpenJ9 PR #5985
   cg()->comp()->failCompilation<TR::AssertionFailure>("UnresolvedDataSnippet");
   return NULL;
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::UnresolvedDataSnippet * snippet)
   {
   uint8_t            *cursor = snippet->getSnippetLabel()->getCodeLocation();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), cursor, "Unresolved Data Snippet");

   //TODO print snippet body
   }

uint32_t
J9::ARM64::UnresolvedDataSnippet::getLength(int32_t estimatedSnippetStart)
   {
   // Implement this in OpenJ9 PR #5985
   cg()->comp()->failCompilation<TR::AssertionFailure>("UnresolvedDataSnippet");
   return 0;
   }
