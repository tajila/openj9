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
#if !defined(CRIUSUPPORT_HPP_)
#define CRIUSUPPORT_HPP_

#include "j9cfg.h"

#if defined(OMR_OVERRIDE_COMPRESS_OBJECT_REFERENCES)
#if OMR_OVERRIDE_COMPRESS_OBJECT_REFERENCES
#define VM_CRIUSupport VM_CRIUSupportCompressed
#else /* OMR_OVERRIDE_COMPRESS_OBJECT_REFERENCES */
#define VM_CRIUSupport VM_CRIUSupportFull
#endif /* OMR_OVERRIDE_COMPRESS_OBJECT_REFERENCES */
#endif /* OMR_OVERRIDE_COMPRESS_OBJECT_REFERENCES */

#include "j9.h"


#include "ObjectAccessBarrierAPI.hpp"
#include "VMHelpers.hpp"
#include "vm_api.h"

class VM_CRIUSupport {
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
	static void
	storeRandomSeedOffsets(J9VMThread *currentThread, )
	{
		J9JavaVM *vm = currentThread->javaVM;

		vm->memoryManagerFunctions->j9mm_iterate_all_objects(vm, vm->portLibrary, 0, iterateThroughHeapCallback, &iteratorData);
	}
protected:

public:
};

#endif /* CRIUSUPPORT_HPP_ */
