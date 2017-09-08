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
#include "j9.h"

#ifndef JITBUILDERHELPER_HPP_
#define JITBUILDERHELPER_HPP_

enum JITBuilderHelperReturnCode {
	NO_ERROR,
	OOM,
	INTERNAL_ERROR
};

class JITBuilderHelper {
public:
	JITBuilderHelperReturnCode generateJavaMethodCallback(J9VMThread *vmThread, jobject anonInstance, jmethodID methodID, J9Class *returnType, J9Class **paramTypes, UDATA typeLen, I_64 *address);
};

#endif /* JAVAMETHODCALLBACK_HPP_ */
