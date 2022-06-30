/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
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
#if !defined(CONTINUATIONHELPERS_HPP_)
#define CONTINUATIONHELPERS_HPP_

#include "j9.h"

class VM_ContinuationHelpers {
	/*
	 * Data members
	 */
private:

protected:

public:

	/*
	 * Function members
	 */
private:

protected:

public:

	static VMINLINE void
	swapFieldsWithContinuation(J9VMThread *vmThread, J9VMContinuation *continuation)
	{
	/* Helper macro to swap fields between the two J9Class structs. */
#define SWAP_MEMBER(fieldName, fieldType, class1, class2) \
	do { \
		fieldType temp = (fieldType) (class1)->fieldName; \
		(class1)->fieldName = (class2)->fieldName; \
		(class2)->fieldName = (fieldType) temp; \
	} while (0)

		SWAP_MEMBER(arg0EA, UDATA*, vmThread, continuation);
		SWAP_MEMBER(bytecodes, UDATA*, vmThread, continuation);
		SWAP_MEMBER(sp, UDATA*, vmThread, continuation);
		SWAP_MEMBER(pc, U_8*, vmThread, continuation);
		SWAP_MEMBER(literals, J9Method*, vmThread, continuation);
		SWAP_MEMBER(stackOverflowMark, UDATA*, vmThread, continuation);
		SWAP_MEMBER(stackOverflowMark2, UDATA*, vmThread, continuation);
		SWAP_MEMBER(stackObject, J9JavaStack*, vmThread, continuation);
	}

};

#endif /* CONTINUATIONHELPERS_HPP_ */
