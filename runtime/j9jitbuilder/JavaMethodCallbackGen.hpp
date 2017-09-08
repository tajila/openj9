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
 
#ifndef JAVAMETHODCALLBACK_HPP_
#define JAVAMETHODCALLBACK_HPP_

#include "j9.h"

#include "JITBuilderHelper.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

class JavaMethodCallbackGen : public TR::MethodBuilder
{
/* fields */
	private:
	J9VMThread *_vmThread;
	J9JavaVM *_vm;
	jobject _anonInstance;
	jmethodID _methodID;
	J9Class * _returnType;
	J9Class **_paramTypes;
	UDATA _paramTypeLen;
	TR::TypeDictionary *_dictionary;
	U_16 *_paramIdentifiers;

/* methods */
	private:
	char* getCallInFunctionName();
	char* getCallInFunctionLineNumber();
	void* getCallInFunctionAddress();
	char* getSignatureFromJ9Class(J9Class *type);
	TR::IlType* getILTypeFromJ9Class(J9Class *type);

	public:
	JavaMethodCallbackGen(TR::TypeDictionary *typeDictionary);
	JITBuilderHelperReturnCode init(J9VMThread *vmThread, jobject anonInstance, jmethodID methodID, J9Class *returnType, J9Class **paramTypes, UDATA typeLen);
	void destroy();
	virtual bool buildIL();

};


#endif /* JAVAMETHODCALLBACK_HPP_ */
