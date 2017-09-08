/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
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
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/


#include "OutOfLineINL.hpp"

#include "vm_internal.h"
#include "vm_api.h"
#include "stackwalk.h"

#include "BytecodeAction.hpp"
#include "UnsafeAPI.hpp"
#include "j9vmnls.h"

#ifdef J9VM_OPT_PANAMA
#include "FFITypeHelpers.hpp"
#include "JITBuilderHelper.hpp"
#endif



extern "C" {


#define SIGNATURE_LENGTH 128

#ifdef J9VM_OPT_PANAMA
/* java.lang.invoke.NativeMethodHandle: private native void initJ9NativeCalloutDataRef(java.lang.String[] argLayoutStrings); */
VM_BytecodeAction
OutOfLineINL_java_lang_invoke_NativeMethodHandle_initJ9NativeCalloutDataRef(J9VMThread *currentThread, J9Method *method)
{
	VM_BytecodeAction rc = EXECUTE_BYTECODE;
	J9JavaVM *vm = currentThread->javaVM;
	ffi_type **args = NULL;
	ffi_cif *cif = NULL;
	J9NativeCalloutData *nativeCalloutData = NULL;

	j9object_t methodHandle = *(j9object_t*)(currentThread->sp + 1);

	Assert_VM_Null(J9VMJAVALANGINVOKENATIVEMETHODHANDLE_J9NATIVECALLOUTDATAREF(currentThread, methodHandle));

	j9object_t methodType = J9VMJAVALANGINVOKEMETHODHANDLE_TYPE(currentThread, methodHandle);
	J9Class *returnTypeClass = J9VM_J9CLASS_FROM_HEAPCLASS(currentThread, J9VMJAVALANGINVOKEMETHODTYPE_RETURNTYPE(currentThread, methodType));
	j9object_t argTypesObject = J9VMJAVALANGINVOKEMETHODTYPE_ARGUMENTS(currentThread, methodType);

	FFITypeHelpers FFIHelpers = FFITypeHelpers(currentThread);
	j9object_t argLayoutStringsObject = *(j9object_t*)currentThread->sp;
	bool layoutStringsExist = false;
	if (NULL != argLayoutStringsObject) {
		layoutStringsExist = true;
	}
	
	PORT_ACCESS_FROM_JAVAVM(vm);

	/**
	 * args[0] stores the ffi_type of the return argument of the method. The remaining args array stores the ffi_type of the input arguments.
	 * typeCount is the number of input arguments, plus one for the return argument.
	 */
	U_32 typeCount = J9INDEXABLEOBJECT_SIZE(currentThread, argTypesObject) + 1;
	args = (ffi_type **)j9mem_allocate_memory(sizeof(ffi_type *) * typeCount, OMRMEM_CATEGORY_VM);
	if (NULL == args) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setNativeOutOfMemoryError(currentThread, 0, 0);
		goto done;
	}

	cif = (ffi_cif *)j9mem_allocate_memory(sizeof(ffi_cif), OMRMEM_CATEGORY_VM);
	if (NULL == cif) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setNativeOutOfMemoryError(currentThread, 0, 0);
		goto freeArgsThenExit;
	}

	/* In the common case we expect that layoutStringsExist will be NULL, which is why args[0] is written first then layoutStringsExist is checked after */
	args[0] = (ffi_type *)FFIHelpers.getFFIType(returnTypeClass);
	if (layoutStringsExist) {
		j9object_t retlayoutStringObject = J9JAVAARRAYOFOBJECT_LOAD(currentThread, argLayoutStringsObject, 0);
		if (NULL != retlayoutStringObject) {
			UDATA structSize = FFIHelpers.getCustomFFIType(&(args[0]), retlayoutStringObject);
			if ((NULL == args[0]) || (NULL == args[0]->elements) || (0 == structSize)) {
				rc = GOTO_THROW_CURRENT_EXCEPTION;
				setNativeOutOfMemoryError(currentThread, 0, 0);
				FFIHelpers.freeStructFFIType(args[0]);
				goto freeCifAndArgsThenExit;
			}
		}
	}

	if (layoutStringsExist) {
		for (U_8 i = 0; i < typeCount-1; i++) {
			j9object_t layoutStringObject = J9JAVAARRAYOFOBJECT_LOAD(currentThread, argLayoutStringsObject, i+1);
			if (NULL == layoutStringObject) {
				args[i+1] = (ffi_type *)FFIHelpers.getFFIType(J9VM_J9CLASS_FROM_HEAPCLASS(currentThread, J9JAVAARRAYOFOBJECT_LOAD(currentThread, argTypesObject, i)));
			} else {
				/**
				 * Pointers are converted to longs before the callout so the ffi type for pointers is sint64.
				 * Structs are also converted to longs before the callout, and are identified by the non-null layout string of the argument.
				 * The struct ffi type is created after the struct layout string is parsed in getCustomFFIType(ffi_type**, j9object_t).
				 */
				UDATA structSize = FFIHelpers.getCustomFFIType(&(args[i+1]), layoutStringObject);
				if (UDATA_MAX == structSize) {
					rc = GOTO_THROW_CURRENT_EXCEPTION;
					setCurrentException(currentThread, J9VMCONSTANTPOOL_JAVALANGINTERNALERROR, NULL);
					goto freeAllMemoryThenExit;
				}
			}
		}
	} else {
		for (U_8 i = 0; i < typeCount-1; i++) {
			args[i+1] = (ffi_type *)FFIHelpers.getFFIType(J9VM_J9CLASS_FROM_HEAPCLASS(currentThread, J9JAVAARRAYOFOBJECT_LOAD(currentThread, argTypesObject, i)));
		}
	}

	/* typeCount-1 because ffi_prep_cif expects the number of input arguments of the method */
	if (FFI_OK != ffi_prep_cif(cif, FFI_DEFAULT_ABI, typeCount-1, args[0], &(args[1]))) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setCurrentException(currentThread, J9VMCONSTANTPOOL_JAVALANGINTERNALERROR, NULL);
		goto freeAllMemoryThenExit;
	}

	nativeCalloutData = (J9NativeCalloutData *)j9mem_allocate_memory(sizeof(J9NativeCalloutData), OMRMEM_CATEGORY_VM);
	if (NULL == nativeCalloutData) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setNativeOutOfMemoryError(currentThread, 0, 0);
		goto freeAllMemoryThenExit;
	}
	nativeCalloutData->arguments = args;
	nativeCalloutData->cif = cif;

	J9VMJAVALANGINVOKENATIVEMETHODHANDLE_SET_J9NATIVECALLOUTDATAREF(currentThread, methodHandle, (void *)nativeCalloutData);

done:
	VM_OutOfLineINL_Helpers::returnVoid(currentThread, 2);
	return rc;
freeAllMemoryThenExit:
	for (U_32 i = 0; i < typeCount; i++) {
		FFIHelpers.freeStructFFIType(args[i]);
	}
freeCifAndArgsThenExit:
	j9mem_free_memory(cif);
freeArgsThenExit:
	j9mem_free_memory(args);
	goto done;
}

/* java.lang.invoke.NativeMethodHandle: private native void freeJ9NativeCalloutDataRef(); */
VM_BytecodeAction
OutOfLineINL_java_lang_invoke_NativeMethodHandle_freeJ9NativeCalloutDataRef(J9VMThread *currentThread, J9Method *method)
{
	J9JavaVM *vm = currentThread->javaVM;
	j9object_t methodHandle = *(j9object_t*)currentThread->sp;

	j9object_t methodType = J9VMJAVALANGINVOKEMETHODHANDLE_TYPE(currentThread, methodHandle);
	j9object_t argTypesObject = J9VMJAVALANGINVOKEMETHODTYPE_ARGUMENTS(currentThread, methodType);
	U_32 argumentCount = J9INDEXABLEOBJECT_SIZE(currentThread, argTypesObject) + 1;
	J9NativeCalloutData *nativeCalloutData = J9VMJAVALANGINVOKENATIVEMETHODHANDLE_J9NATIVECALLOUTDATAREF(currentThread, methodHandle);

	/* Manually synchronize methodHandle since declaring the method as synchronized in Java will have no effect (for all INLs). */
	j9object_t methodHandleLock = (j9object_t)objectMonitorEnter(currentThread, methodHandle);
	if (NULL == methodHandleLock) {
		setNativeOutOfMemoryError(currentThread, J9NLS_VM_FAILED_TO_ALLOCATE_MONITOR);
		goto done;
	}

	J9VMJAVALANGINVOKENATIVEMETHODHANDLE_SET_J9NATIVECALLOUTDATAREF(currentThread, methodHandle, NULL);

	if (NULL != nativeCalloutData) {
		PORT_ACCESS_FROM_JAVAVM(vm);

		Assert_VM_notNull(nativeCalloutData);
		Assert_VM_notNull(nativeCalloutData->arguments);
		Assert_VM_notNull(nativeCalloutData->cif);

		FFITypeHelpers FFIHelpers = FFITypeHelpers(currentThread);
		for (U_8 i = 0; i < argumentCount; i++) {
			FFIHelpers.freeStructFFIType(nativeCalloutData->arguments[i]);
		}
		j9mem_free_memory(nativeCalloutData->arguments);
		nativeCalloutData->arguments = NULL;

		j9mem_free_memory(nativeCalloutData->cif);
		nativeCalloutData->cif = NULL;

		j9mem_free_memory(nativeCalloutData);
		nativeCalloutData = NULL;
	}

	objectMonitorExit(currentThread, methodHandleLock);

done:
	VM_OutOfLineINL_Helpers::returnVoid(currentThread, 1);
	return EXECUTE_BYTECODE;
}

/* jdk.internal.nicl.NativeInvoker:
 * private final static native long generateNativeFunctionPointerBinding(
 * 																			MethodType mt,
 * 																			Class<?> returnType,
 * 																			Class<?>[] parameterTypes,
 * 																			Class<? extends Object> anonClass,
 * 																			Object anonInstance,
 * 																			String methodSignature
 * 																		)
 */

VM_BytecodeAction
OutOfLineINL_jdk_internal_nicl_NativeInvoker_generateNativeFunctionPointerBinding(J9VMThread *currentThread, J9Method *method)
{
	J9JavaVM *vm = currentThread->javaVM;
	j9object_t methodSignatureObj = *(j9object_t*)currentThread->sp;
	jobject anonInstance = (jobject)(currentThread->sp + 1);
	jclass anonClassRef = (jclass)(currentThread->sp + 2);
	j9object_t paramTypeArrayObj = *(j9object_t*)(currentThread->sp + 3);
	j9object_t returnTypeObj = *(j9object_t*)(currentThread->sp + 4);
	j9object_t methodType = *(j9object_t*)(currentThread->sp + 5);
	J9InternalVMFunctions* vmFuncs = vm->internalVMFunctions;
	VM_BytecodeAction rc = EXECUTE_BYTECODE;
	I_64 functionAddress = 0;
	char buf[SIGNATURE_LENGTH];
	char *signature = buf;
	U_16 length = 0;
	jmethodID methodID = NULL;
	J9Class *typeBuf[17];
	J9Class **paramTypes = typeBuf;
	U_32 paramSize = 0;
	JITBuilderHelperReturnCode retCode = NO_ERROR;
	UDATA *spPriorToFrameBuild = currentThread->sp;
	J9SFMethodTypeFrame *frame = buildMethodTypeFrame(currentThread, methodType);

	length = (U_16)vmFuncs->getStringUTF8Length(currentThread, methodSignatureObj);
	if (length > SIGNATURE_LENGTH) {
		PORT_ACCESS_FROM_VMC(currentThread);
		signature = (char *) j9mem_allocate_memory(sizeof(char) * (length + 1) , OMRMEM_CATEGORY_VM);
		if (NULL == signature) {
			rc = GOTO_THROW_CURRENT_EXCEPTION;
			setNativeOutOfMemoryError(currentThread, 0, 0);
			goto popMethodTypeFrame;
		}
	}

	if (UDATA_MAX == vmFuncs->copyStringToUTF8Helper(currentThread, methodSignatureObj, TRUE, J9_STR_NONE, (U_8*) signature, length + 1)) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setCurrentException(currentThread, J9VMCONSTANTPOOL_JAVALANGINTERNALERROR, NULL);
		goto popMethodTypeFrame;
	}

	/* can this release vmAccess ? TODO */
	methodID = (jmethodID) getMethodOrFieldID((JNIEnv *) currentThread, anonClassRef, "fn", signature, J9JNIID_METHOD, FALSE);
	if (NULL == methodID) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		setCurrentException(currentThread, J9VMCONSTANTPOOL_JAVALANGINTERNALERROR, NULL);
		goto freeSignature;
	}

	paramSize = J9INDEXABLEOBJECT_SIZE(currentThread, paramTypeArrayObj);

	if (paramSize > 16) {
		PORT_ACCESS_FROM_VMC(currentThread);
		paramTypes = (J9Class **) j9mem_allocate_memory(sizeof(J9Class *) * paramSize , OMRMEM_CATEGORY_VM);
		if (NULL == paramTypes) {
			rc = GOTO_THROW_CURRENT_EXCEPTION;
			setNativeOutOfMemoryError(currentThread, 0, 0);
			goto freeSignature;
		}
	}

	for (UDATA i = 0; i < paramSize; i++) {
		paramTypes[i] = J9VM_J9CLASS_FROM_HEAPCLASS(currentThread, J9JAVAARRAYOFOBJECT_LOAD(currentThread, paramTypeArrayObj, i));
	}

	JITBuilderHelper jitHelper;
	retCode = jitHelper.generateJavaMethodCallback(currentThread,
										anonInstance,
										methodID,
										J9VM_J9CLASS_FROM_HEAPCLASS(vm, returnTypeObj),
										paramTypes,
										paramSize,
										&functionAddress);
	if (NO_ERROR != retCode) {
		rc = GOTO_THROW_CURRENT_EXCEPTION;
		if (OOM == retCode) {
			setNativeOutOfMemoryError(currentThread, 0, 0);
		} else {
			setCurrentException(currentThread, J9VMCONSTANTPOOL_JAVALANGINTERNALERROR, NULL);
		}
	}


goto freeParamTypes;

done:
	VM_OutOfLineINL_Helpers::returnDouble(currentThread, (I_64) functionAddress, 5);
	return rc;

freeParamTypes:
	if (paramTypes != typeBuf) {
		PORT_ACCESS_FROM_VMC(currentThread);
		j9mem_free_memory(paramTypes);
	}

freeSignature:
	if (buf != signature) {
		PORT_ACCESS_FROM_VMC(currentThread);
		j9mem_free_memory(signature);
	}

popMethodTypeFrame:
	currentThread->literals = frame->savedCP;
	currentThread->pc = frame->savedPC;
	currentThread->arg0EA = UNTAGGED_A0(frame);
	currentThread->sp = spPriorToFrameBuild;
	goto done;

}
#endif /* J9VM_OPT_PANAMA */
}



