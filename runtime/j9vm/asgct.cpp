/*******************************************************************************
 * Copyright (c) 2021, 2021 IBM Corp. and others
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

#include "j9.h"
#include "j9protos.h"
#include "ut_j9scar.h"
#include "rommeth.h"

#if defined(LINUX) && defined(J9VM_ARCH_X86) && defined(J9VM_ENV_DATA64)
#include <ucontext.h>
#define ASGCT_SUPPORTED
#endif /* defined(LINUX) && defined(J9VM_ARCH_X86) && defined(J9VM_ENV_DATA64) */

extern "C" {

#define AGCT_LINE_NUMBER_NATIVE_METHOD -3

enum {
ticks_no_Java_frame         =  0, // new thread
ticks_no_class_load         = -1, // jmethodIds are not available
ticks_GC_active             = -2, // GC action
ticks_unknown_not_Java      = -3, // ¯\_(ツ)_/¯
ticks_not_walkable_not_Java = -4, // ¯\_(ツ)_/¯
ticks_unknown_Java          = -5, // ¯\_(ツ)_/¯
ticks_not_walkable_Java     = -6, // ¯\_(ツ)_/¯
ticks_unknown_state         = -7, // ¯\_(ツ)_/¯
ticks_thread_exit           = -8, // dying thread
ticks_deopt                 = -9, // mid-deopting code
ticks_safepoint             = -10 // ¯\_(ツ)_/¯
}; 

typedef struct {
	jint lineno;
	jmethodID method_id;
} ASGCT_CallFrame;

typedef struct {
	JNIEnv *env_id;
	jint num_frames;
	ASGCT_CallFrame *frames;
} ASGCT_CallTrace;

#if defined(ASGCT_SUPPORTED)

extern J9JavaVM *BFUjavaVM;

static UDATA
asyncFrameIterator(J9VMThread * currentThread, J9StackWalkState * walkState)
{
	ASGCT_CallFrame *frame = (ASGCT_CallFrame*)walkState->userData1;
	J9Method *method = walkState->method;
	J9JavaVM * vm = currentThread->javaVM;
	J9Class * declaringClass = J9_CLASS_FROM_METHOD(method);
	UDATA methodIndex = getMethodIndexUnchecked(method);
	if (UDATA_MAX == methodIndex) {
		*(UDATA*)UDATA_MAX = UDATA_MAX;		
	}
	void ** jniIDs = declaringClass->jniIDs;
	if (NULL == jniIDs) {
		*(UDATA*)UDATA_MAX = UDATA_MAX;		
	}
	J9JNIMethodID * methodID = (J9JNIMethodID*)(jniIDs[methodIndex]);
	if (NULL == methodID) {
		*(UDATA*)UDATA_MAX = UDATA_MAX;
	}
	frame->method_id = (jmethodID)methodID;
	J9ROMMethod *romMethod = J9_ROM_METHOD_FROM_RAM_METHOD(method);
	if (J9_ARE_ANY_BITS_SET(romMethod->modifiers, J9AccNative)) {
		frame->lineno = AGCT_LINE_NUMBER_NATIVE_METHOD;
	} else {
		frame->lineno = (jint)walkState->bytecodePCOffset;
	}
	walkState->userData1 = (void*)(frame + 1);
	return J9_STACKWALK_KEEP_ITERATING;
}

static UDATA
emptySignalHandler(J9PortLibrary *portLibrary, U_32 gpType, void *gpInfo, void *handler_arg)
{
   return J9PORT_SIG_EXCEPTION_RETURN;
}

typedef struct {
	ASGCT_CallTrace *trace;
	jint depth;
	void *ucontext;
	J9VMThread *currentThread;
	jint num_frames;
	U_8 *pc;
	UDATA *sp;
	UDATA *arg0EA;
	J9Method *literals;
} ASGCT_parms;

static UDATA
protectedASGCT(J9PortLibrary *portLib, void *arg)
{
	ASGCT_parms *parms = (ASGCT_parms*)arg;
	J9VMThread *currentThread = BFUjavaVM->internalVMFunctions->currentVMThread(BFUjavaVM);
	if (NULL != currentThread) {
		PORT_ACCESS_FROM_VMC(currentThread);
		bool jit = false;
		parms->currentThread = currentThread;
		parms->pc = currentThread->pc;
		parms->sp = currentThread->sp;
		parms->arg0EA = currentThread->arg0EA;
		parms->literals = currentThread->literals;
		parms->num_frames = ticks_not_walkable_Java;
		J9JITConfig *jitConfig = BFUjavaVM->jitConfig;
		J9StackWalkState walkState = {0};
		if (NULL != jitConfig) {
			void *ucontext = parms->ucontext;
			if (NULL != ucontext) {
				greg_t *regs = ((ucontext_t*)ucontext)->uc_mcontext.gregs;
				greg_t rip = regs[REG_RIP];
				J9JITExceptionTable *metaData = jitConfig->jitGetExceptionTableFromPC(currentThread, rip);
				if (NULL != metaData) {
					greg_t rsp = regs[REG_RSP];
					J9SFJITResolveFrame resolveFrame;
					resolveFrame.savedJITException = NULL;
					resolveFrame.specialFrameFlags = J9_SSF_JIT_RESOLVE;
					resolveFrame.parmCount = 0;
					resolveFrame.returnAddress = (U_8*)rip;
					resolveFrame.taggedRegularReturnSP = (UDATA *) (((U_8 *)rsp) + J9SF_A0_INVISIBLE_TAG);

					currentThread->pc = (U_8 *) J9SF_FRAME_TYPE_JIT_RESOLVE;
					currentThread->arg0EA = (UDATA *)&(resolveFrame.taggedRegularReturnSP);
					currentThread->literals = NULL;
					currentThread->sp = (UDATA *)&resolveFrame;
//					j9tty_printf(PORTLIB, "\n\n!!! JIT case\n");
					jit = true;
				}
			}
		}
		walkState.walkThread = currentThread;
		walkState.skipCount = 0;
		walkState.maxFrames = parms->depth;
		/* JIT cache may allocate, which we don't want inside a signal handler,
		 * Flag is cleared in the caller (so both the normal and exception cases
		 * clear it).
		 */
		currentThread->jitArtifactSearchCache = (void*)((UDATA)currentThread->jitArtifactSearchCache | J9_STACKWALK_NO_JIT_CACHE);
		/* Disable trace and assertions */
		currentThread->privateFlags2 |= J9_PRIVATE_FLAGS2_DISABLE_TRACE;
		walkState.flags = J9_STACKWALK_INCLUDE_NATIVES | J9_STACKWALK_VISIBLE_ONLY
			| J9_STACKWALK_RECORD_BYTECODE_PC_OFFSET | J9_STACKWALK_COUNT_SPECIFIED
			| J9_STACKWALK_ITERATE_FRAMES | J9_STACKWALK_NO_ERROR_REPORT;
		walkState.userData1 = (void*)parms->trace->frames;
		walkState.frameWalkFunction = asyncFrameIterator;
		UDATA result = BFUjavaVM->walkStackFrames(currentThread, &walkState);
		if (J9_STACKWALK_RC_NONE == result) {
			parms->num_frames = (jint)walkState.framesWalked;
			if (jit) {
//				j9tty_printf(PORTLIB, "!!! frames = %d\n", parms->num_frames);
			}
		}
	}
	return 0;
}

#endif /* ASGCT_SUPPORTED */

void AsyncGetCallTrace(ASGCT_CallTrace *trace, jint depth, void *ucontext)
{
	J9VMThread *currentThread = NULL;
	jint num_frames = ticks_unknown_not_Java;
#if defined(ASGCT_SUPPORTED)
	if (NULL != BFUjavaVM) {
		PORT_ACCESS_FROM_JAVAVM(BFUjavaVM);
		ASGCT_parms parms = { trace, depth, ucontext, currentThread, num_frames, NULL, NULL, NULL, NULL };
		UDATA result = 0;
		j9sig_protect(
				protectedASGCT, (void*)&parms, 
				emptySignalHandler, NULL,
				J9PORT_SIG_FLAG_SIGALLSYNC | J9PORT_SIG_FLAG_MAY_RETURN, 
				&result);
		num_frames = parms.num_frames;
		currentThread = parms.currentThread;
		if (NULL != currentThread) {
			/* Restore normal behaviour on the thread */
			currentThread->jitArtifactSearchCache = (void*)((UDATA)currentThread->jitArtifactSearchCache & ~J9_STACKWALK_NO_JIT_CACHE);
			currentThread->privateFlags2 &= ~J9_PRIVATE_FLAGS2_DISABLE_TRACE;
			currentThread->pc = parms.pc;
			currentThread->sp = parms.sp;
			currentThread->arg0EA = parms.arg0EA;
			currentThread->literals = parms.literals;
		}
	}
#endif /* ASGCT_SUPPORTED */
	trace->env_id = (JNIEnv*)currentThread;
	trace->num_frames = num_frames;
}

} /* extern "C" */
