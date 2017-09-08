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
#include "JavaMethodCallbackGen.hpp"

#include <stdlib.h>

#include "j9.h"
#include "ut_j9jitbuilder.h"

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/MethodBuilder.hpp"


JavaMethodCallbackGen::JavaMethodCallbackGen(TR::TypeDictionary *typeDictionary) : MethodBuilder(typeDictionary)
	,_vmThread(NULL)
	,_anonInstance(NULL)
	,_methodID(NULL)
	,_paramTypes(NULL)
	,_paramTypeLen(0)
	,_dictionary(typeDictionary)
	,_returnType(NULL)
	,_vm(NULL)
{}

JITBuilderHelperReturnCode
JavaMethodCallbackGen::init(J9VMThread *vmThread, jobject anonInstance, jmethodID methodID, J9Class *returnType, J9Class **paramTypes, UDATA paramTypeLen)
{
	JITBuilderHelperReturnCode rc = NO_ERROR;
	PORT_ACCESS_FROM_VMC(vmThread);
	_vmThread = vmThread;
	_vm = vmThread->javaVM;
	_anonInstance = anonInstance;
	_methodID = methodID;
	_returnType = returnType;
	_paramTypes = paramTypes;
	_paramTypeLen = paramTypeLen;

	_paramIdentifiers = (U_16 *) j9mem_allocate_memory(sizeof(U_16)*_paramTypeLen, OMRMEM_CATEGORY_VM);
	if (NULL == _paramIdentifiers) {
		rc == OOM;
	} else {
		DefineLine(LINETOSTR(__LINE__));
		DefineFile(__FILE__);

		for (int i = 0; i < _paramTypeLen; i++) {
			snprintf((char*)(_paramIdentifiers + i), sizeof(U_16),"%d\0", i);
			DefineParameter((char*)(_paramIdentifiers + i), getILTypeFromJ9Class(_paramTypes[i]));
		}

		DefineName("JavaMethodCallbackGen");
		DefineReturnType(getILTypeFromJ9Class(returnType));

		DefineFunction((char *)getCallInFunctionName(),
		                  (char *)"jni.h",
		                  getCallInFunctionLineNumber(),
		                  getCallInFunctionAddress(),
						  getILTypeFromJ9Class(_returnType),
		                  3,
						  Address,
						  Address,
						  Address);

		if (_paramTypeLen > 0) {
			_dictionary->DefineUnion("jvalue");
			_dictionary->UnionField("jvalue", "Z", _dictionary->toIlType<uint8_t>());
			_dictionary->UnionField("jvalue", "B", Int8);
			_dictionary->UnionField("jvalue", "C", _dictionary->toIlType<uint16_t>());
			_dictionary->UnionField("jvalue", "S", Int16);
			_dictionary->UnionField("jvalue", "I", Int32);
			_dictionary->UnionField("jvalue", "J", Int64);
			_dictionary->UnionField("jvalue", "F", Float);
			_dictionary->UnionField("jvalue", "D", Double);
			_dictionary->CloseUnion("jvalue");
		}
	}

	return rc;
}

void
JavaMethodCallbackGen::destroy()
{
	PORT_ACCESS_FROM_VMC(_vmThread);
	j9mem_free_memory(_paramIdentifiers);
}

bool
JavaMethodCallbackGen::buildIL()
{
	if (_paramTypeLen > 0) {
		TR::IlType *jValueType = _dictionary->LookupUnion("jvalue");
		TR::IlType *pJvalue = _dictionary->PointerTo(jValueType);

		/* create array of jvalues (length of parameters) on stack */
		Store("parameters",
				CreateLocalArray(_paramTypeLen, jValueType));

		for (int i = 0; i < _paramTypeLen; i++) {
			/* get pointer to jvalue at parameter index */
			Store("param", IndexAt(pJvalue,
			         Load("parameters"),
			         ConstInt32(i)));

			/* fill jvalue with zero first */
			StoreIndirect("jvalue", "J",
								Load("param"),
								ConstInt64(0));

			/* load parameter into jvalue */
			StoreIndirect("jvalue", getSignatureFromJ9Class(_paramTypes[i]),
					Load("param"),
					Load((char*)(_paramIdentifiers + i)));

		}


	} else {
		Store("parameters", ConstAddress(NULL));
	}

	if (getILTypeFromJ9Class(_returnType) == NoType) {
		Call(getCallInFunctionName(), 4, ConstAddress((void*)_vmThread), ConstAddress((void*) _anonInstance), ConstAddress(_methodID), Load("parameters"));
		Return();
	} else {
		Return(Call(getCallInFunctionName(), 4, ConstAddress((void*)_vmThread), ConstAddress((void*) _anonInstance), ConstAddress(_methodID), Load("parameters")));
	}

	return true;
}

void*
JavaMethodCallbackGen::getCallInFunctionAddress()
{
	void *functionAddress = NULL;
	if (_returnType == _vm->voidReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallVoidMethodA;
	} else if (_returnType == _vm->booleanReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallBooleanMethodA;
	} else if (_returnType == _vm->charReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallCharMethodA;
	} else if (_returnType == _vm->floatReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallFloatMethodA;
	} else if (_returnType == _vm->doubleReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallDoubleMethodA;
	} else if (_returnType == _vm->byteReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallByteMethodA;
	} else if (_returnType == _vm->shortReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallShortMethodA;
	} else if (_returnType == _vm->intReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallIntMethodA;
	} else if (_returnType == _vm->longReflectClass) {
		functionAddress = (void*) _vmThread->functions->CallLongMethodA;
	} else {
		Assert_J9JITBUILDER_unreachable();
	}

	return functionAddress;
}

char*
JavaMethodCallbackGen::getCallInFunctionName()
{
	char* name = NULL;
	if (_returnType == _vm->voidReflectClass) {
		name = (char*) "CallVoidMethodA";
	} else if (_returnType == _vm->booleanReflectClass) {
		name = (char*) "CallBooleanMethodA";
	} else if (_returnType == _vm->charReflectClass) {
		name = (char*) "CallCharMethodA";
	} else if (_returnType == _vm->floatReflectClass) {
		name = (char*) "CallFloatMethodA";
	} else if (_returnType == _vm->doubleReflectClass) {
		name = (char*) "CallDoubleMethodA";
	} else if (_returnType == _vm->byteReflectClass) {
		name = (char*) "CallByteMethodA";
	} else if (_returnType == _vm->shortReflectClass) {
		name = (char*) "CallShortMethodA";
	} else if (_returnType == _vm->intReflectClass) {
		name = (char*) "CallIntMethodA";
	} else if (_returnType == _vm->longReflectClass) {
		name = (char*) "CallLongMethodA";
	} else {
		Assert_J9JITBUILDER_unreachable();
	}
	return name;
}

char*
JavaMethodCallbackGen::getSignatureFromJ9Class(J9Class *type)
{
	char* name = NULL;
	if (type == _vm->voidReflectClass) {
		name = (char*) "V";
	} else if (type == _vm->booleanReflectClass) {
		name = (char*) "Z";
	} else if (type == _vm->charReflectClass) {
		name = (char*) "C";
	} else if (type == _vm->floatReflectClass) {
		name = (char*) "F";
	} else if (type == _vm->doubleReflectClass) {
		name = (char*) "D";
	} else if (type == _vm->byteReflectClass) {
		name = (char*) "B";
	} else if (type == _vm->shortReflectClass) {
		name = (char*) "S";
	} else if (type == _vm->intReflectClass) {
		name = (char*) "I";
	} else if (type == _vm->longReflectClass) {
		name = (char*) "J";
	} else {
		Assert_J9JITBUILDER_unreachable();
	}
	return name;
}

char*
JavaMethodCallbackGen::getCallInFunctionLineNumber()
{
	char *line = NULL;
	if (_returnType == _vm->voidReflectClass) {
		line = (char*) "258";
	} else if (_returnType == _vm->booleanReflectClass) {
		line = (char*) "234";
	} else if (_returnType == _vm->charReflectClass) {
		line = (char*) "240";
	} else if (_returnType == _vm->floatReflectClass) {
		line = (char*) "252";
	} else if (_returnType == _vm->doubleReflectClass) {
		line = (char*) "255";
	} else if (_returnType == _vm->byteReflectClass) {
		line = (char*) "237";
	} else if (_returnType == _vm->shortReflectClass) {
		line = (char*) "243";
	} else if (_returnType == _vm->intReflectClass) {
		line = (char*) "246";
	} else if (_returnType == _vm->longReflectClass) {
		line = (char*) "249";
	} else {
		Assert_J9JITBUILDER_unreachable();
	}
	return line;
}

TR::IlType*
JavaMethodCallbackGen::getILTypeFromJ9Class(J9Class *type)
{
	TR::IlType *typeCode = NULL;
	if (type == _vm->voidReflectClass) {
		typeCode = NoType;
	} else if (type == _vm->booleanReflectClass) {
		typeCode = _dictionary->toIlType<uint8_t>();
	} else if (type == _vm->charReflectClass) {
		typeCode = _dictionary->toIlType<uint16_t>();
	} else if (type == _vm->floatReflectClass) {
		typeCode = Float;
	} else if (type == _vm->doubleReflectClass) {
		typeCode = Double;
	} else if (type == _vm->byteReflectClass) {
		typeCode = Int8;
	} else if (type == _vm->shortReflectClass) {
		typeCode = Int16;
	} else if (type == _vm->intReflectClass) {
		typeCode = Int32;
	} else if (type == _vm->longReflectClass) {
		typeCode = Int64;
	} else {
		Assert_J9JITBUILDER_unreachable();
	}
	return typeCode;
}
