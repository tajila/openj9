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
#include "ValueTypeHelpers.hpp"

#include "j9.h"
#include "ut_j9vm.h"
#include "HeapIteratorAPI.h"
#include "ObjectAccessBarrierAPI.hpp"
#include "vm_api.h"
#include <stdlib.h>

#define RANDDOM_CLASS_NAME ((U_8*)"java/util/Random")
#define RANDDOM_CLASS_NAME_LENGTH strlen("java/util/Random")
J9_DECLARE_CONSTANT_UTF8(java_util_Random_seed, "seed");
J9_DECLARE_CONSTANT_UTF8(java_util_Random_seedSig, "Ljava/util/concurrent/atomic/AtomicLong;");
J9_DECLARE_CONSTANT_UTF8(java_util_concurrent_atomic_AtomicLong, "java/util/concurrent/atomic/AtomicLong");
J9_DECLARE_CONSTANT_UTF8(java_util_concurrent_atomic_AtomicLong_value, "value");
J9_DECLARE_CONSTANT_UTF8(java_util_concurrent_atomic_AtomicLong_valueSig, "J");

//typedef struct CRIUFixupData {
//	J9VMThread *currentThread;
//	J9Class *randomSeedClass;
//	J9Class *atomicLongClass;
//	UDATA randomSeedOffset; /* Random Seed */
//	UDATA atomicLongValueOffset; /* Atomic Long */
//	J9Pool *randomSeedOffsets;
//} CRIUFixupData;


extern "C" {

	jvmtiIterationControl
	storeRandomInstancesSeedImpl(J9JavaVM *vm, J9MM_IterateObjectDescriptor *objDesc, void *state)
	{
		CRIUFixupData *fixupData = (CRIUFixupData *) state;
		j9object_t obj = objDesc->object;

		if (J9OBJECT_CLAZZ_VM(vm, obj) == fixupData->randomSeedClass) {
			j9object_t *savedObj = (j9object_t *)pool_newElement(fixupData->randomSeedInstances);
			*savedObj = obj;
		}
		return JVMTI_ITERATION_CONTINUE;
	}

	BOOLEAN
	storeRandomSeedOffsets(J9VMThread *currentThread)
	{
		J9JavaVM *vm = currentThread->javaVM;
		J9Pool *randomSeedInstances = pool_new(sizeof(UDATA),  0, 0, 0, J9_GET_CALLSITE(), J9MEM_CATEGORY_JNI, POOL_FOR_PORT(vm->portLibrary));
		J9Class *randomClass = internalFindClassUTF8(currentThread, RANDDOM_CLASS_NAME, RANDDOM_CLASS_NAME_LENGTH, vm->systemClassLoader, J9_FINDCLASS_FLAG_EXISTING_ONLY);
		BOOLEAN rc = TRUE;

		if (NULL == randomSeedInstances) {
			vm->internalVMFunctions->setNativeOutOfMemoryError(currentThread, 0, 0);
			rc = FALSE;
			goto done;
		}

		if (NULL != randomClass) {
			J9Class *definingClass = NULL;
			J9ROMFieldShape *field = NULL;
			PORT_ACCESS_FROM_JAVAVM(vm);

			CRIUFixupData *fixupData = (CRIUFixupData*)j9mem_allocate_memory(sizeof(CRIUFixupData), J9MEM_CATEGORY_JNI);
			if (NULL == fixupData) {
				vm->internalVMFunctions->setNativeOutOfMemoryError(currentThread, 0, 0);
				rc = FALSE;
				goto done;
			}

			fixupData->atomicLongClass = internalFindClassUTF8(currentThread, J9UTF8_DATA(&java_util_concurrent_atomic_AtomicLong), J9UTF8_LENGTH(&java_util_concurrent_atomic_AtomicLong), vm->systemClassLoader, J9_FINDCLASS_FLAG_EXISTING_ONLY);
			if (NULL == fixupData->atomicLongClass) {
				rc = FALSE;
				goto done;
			}
			fixupData->randomSeedClass = randomClass;
			fixupData->randomSeedOffset = instanceFieldOffset(currentThread, randomClass, J9UTF8_DATA(&java_util_Random_seed), J9UTF8_LENGTH(&java_util_Random_seed), J9UTF8_DATA(&java_util_Random_seedSig), J9UTF8_LENGTH(&java_util_Random_seedSig), &definingClass, (UDATA *)&field, 0);
			fixupData->atomicLongValueOffset = instanceFieldOffset(currentThread, fixupData->atomicLongClass, J9UTF8_DATA(&java_util_concurrent_atomic_AtomicLong_value), J9UTF8_LENGTH(&java_util_concurrent_atomic_AtomicLong_value), J9UTF8_DATA(&java_util_concurrent_atomic_AtomicLong_valueSig), J9UTF8_LENGTH(&java_util_concurrent_atomic_AtomicLong_valueSig), &definingClass, (UDATA *)&field, 0);
			fixupData->currentThread = currentThread;
			fixupData->randomSeedInstances = randomSeedInstances;

			vm->memoryManagerFunctions->j9mm_iterate_all_objects(vm, vm->portLibrary, 0, storeRandomInstancesSeedImpl, fixupData);

			vm->criuFixupData = fixupData;
		}
done:
		return rc;
	}

	BOOLEAN
	resetRandomSeedOffsets(J9VMThread *currentThread)
	{
		J9JavaVM *vm = currentThread->javaVM;
		CRIUFixupData *fixupData = vm->criuFixupData;
		PORT_ACCESS_FROM_JAVAVM(vm);
		BOOLEAN rc = TRUE;

		if (NULL != fixupData) {
			UDATA const objectHeaderSize = J9VMTHREAD_OBJECT_HEADER_SIZE(fixupData->currentThread);
			pool_state randomSeedOffsetsWalkState = {0};
			MM_ObjectAccessBarrierAPI objectAccessBarrier(currentThread);
			MM_ObjectAllocationAPI objectAllocate(currentThread);
			UDATA success = 0;
			srand(j9time_current_time_nanos(&success));
			if (!success) {
				rc = FALSE;
				goto done;
			}
			j9object_t *savedObj = (j9object_t*)pool_startDo(fixupData->randomSeedInstances, &randomSeedOffsetsWalkState);
			while (NULL != savedObj) {
				j9object_t obj = objectAllocate.inlineAllocateObject(currentThread, fixupData->atomicLongClass, false, false);
				if (NULL == obj) {
					obj = vm->memoryManagerFunctions->J9AllocateObject(currentThread, fixupData->atomicLongClass, J9_GC_ALLOCATE_OBJECT_NON_INSTRUMENTABLE);
					if (NULL == obj) {
						vm->internalVMFunctions->setHeapOutOfMemoryError(currentThread);
						rc = FALSE;
						goto done;
					}
				}

				double randVal = rand();
				U_64 randLong = (U_64)(randVal * (U_64_MAX/U_32_MAX));

				objectAccessBarrier.inlineMixedObjectStoreU64(currentThread, obj, fixupData->atomicLongValueOffset + objectHeaderSize, randLong, true);
				objectAccessBarrier.inlineMixedObjectStoreObject(currentThread, *savedObj, fixupData->randomSeedOffset + objectHeaderSize, obj, false);

				savedObj = (j9object_t*)pool_nextDo(&randomSeedOffsetsWalkState);
			}
		}

done:
		return rc;

	}

}
