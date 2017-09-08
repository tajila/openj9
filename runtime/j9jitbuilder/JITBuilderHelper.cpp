/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code is also Distributed under one or more Secondary Licenses,
 * as those terms are defined by the Eclipse Public License, v. 2.0: GNU
 * General Public License, version 2 with the GNU Classpath Exception [1]
 * and GNU General Public License, version 2 with the OpenJDK Assembly
 * Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *******************************************************************************/
#include "JITBuilderHelper.hpp"

#include "Jit.hpp"
#include "JavaMethodCallbackGen.hpp"


JITBuilderHelperReturnCode
JITBuilderHelper::generateJavaMethodCallback(J9VMThread *vmThread, jobject anonInstance, jmethodID methodID, J9Class *returnType, J9Class **paramTypes, UDATA typeLen, I_64 *address)
{
	bool initialized = initializeJit();
	JITBuilderHelperReturnCode rc = NO_ERROR;
	if (initialized) {
		TR::TypeDictionary typeDictionary;
		JavaMethodCallbackGen methodGen(&typeDictionary);
		rc = methodGen.init(vmThread, anonInstance, methodID, returnType, paramTypes, typeLen);
		if (NO_ERROR == rc) {
			if (0 != compileMethodBuilder(&methodGen, (U_8**) address)) {
				rc = INTERNAL_ERROR;
			}
		}
		methodGen.destroy();
		shutdownJit();
	} else {
		rc = INTERNAL_ERROR;
	}

	return rc;
}
