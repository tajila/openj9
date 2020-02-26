/*******************************************************************************
 * Copyright (c) 2001, 2020 IBM Corp. and others
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

#include "VMSnapshotImpl.hpp"
#include "SnapshotFileFormat.h"
#include "segment.h"
#include <errno.h>

#include <sys/mman.h> /* TODO: Change to OMRPortLibrary MMAP functionality. Currently does not allow MAP_FIXED as it is not supported in all architectures */

VMSnapshotImpl::VMSnapshotImpl(J9PortLibrary *portLibrary, const char* ramCache) :
	_vm(NULL),
	_portLibrary(portLibrary),
	_snapshotHeader(NULL),
	_heap(NULL),
	_invalidITable(NULL),
	_ramCache(ramCache),
	_vmSnapshotImplMonitor(NULL),
	_vmSnapshotImplPortLibrary(NULL)
{
}

void
VMSnapshotImpl::setJ9JavaVM(J9JavaVM *vm)
{
	_vm = vm;
}

void
VMSnapshotImpl::setImagePortLib(VMSnapshotImplPortLibrary *vmSnapshotImplPortLibrary)
{
	_vmSnapshotImplPortLibrary = vmSnapshotImplPortLibrary;
}

J9JavaVM *
VMSnapshotImpl::getJ9JavaVM(void)
{
	return _vm;
}

VMSnapshotImpl::~VMSnapshotImpl()
{
	PORT_ACCESS_FROM_JAVAVM(_vm);
	destroyMonitor();
	if (IS_SNAPSHOT_RUN(_vm)) {
		j9mem_free_memory((void*)_snapshotHeader->imageAddress);
	} else {
		munmap((void*)_snapshotHeader, _snapshotHeader->imageAlignedAddress);
	}
}

bool
VMSnapshotImpl::initializeMonitor(void)
{
	if (omrthread_monitor_init_with_name(&_vmSnapshotImplMonitor, 0, "JVM Image Heap Monitor") != 0) {
		return false;
	}

	return true;
}

void
VMSnapshotImpl::storeInitialMethods(J9Method* cInitialStaticMethod, J9Method* cInitialSpecialMethod, J9Method* cInitialVirtualMethod)
{
	WSRP_SET(_snapshotHeader->cInitialStaticMethod, cInitialStaticMethod);
	WSRP_SET(_snapshotHeader->cInitialSpecialMethod, cInitialSpecialMethod);
	WSRP_SET(_snapshotHeader->cInitialVirtualMethod, cInitialVirtualMethod);
}

void
VMSnapshotImpl::setInitialMethods(J9Method** cInitialStaticMethod, J9Method** cInitialSpecialMethod, J9Method** cInitialVirtualMethod)
{
	*cInitialStaticMethod = WSRP_GET(_snapshotHeader->cInitialStaticMethod, J9Method*);
	*cInitialSpecialMethod = WSRP_GET(_snapshotHeader->cInitialSpecialMethod, J9Method*);
	*cInitialVirtualMethod = WSRP_GET(_snapshotHeader->cInitialVirtualMethod, J9Method*);
}

bool
VMSnapshotImpl::initializeInvalidITable(void)
{
	_invalidITable = (J9ITable *) subAllocateMemory(sizeof(J9ITable));
	if (NULL == _invalidITable) {
		return false;
	}

	_invalidITable->interfaceClass = (J9Class *) (UDATA) 0xDEADBEEF;
	_invalidITable->depth = 0;
	_invalidITable->next = (J9ITable *) NULL;

	return true;
}

void
VMSnapshotImpl::destroyMonitor(void)
{
	omrthread_monitor_destroy(_vmSnapshotImplMonitor);
}

VMSnapshotImpl *
VMSnapshotImpl::createInstance(J9PortLibrary *portLibrary, const char* ramCache)
{
	PORT_ACCESS_FROM_PORT(portLibrary);
	VMSnapshotImpl *jvmInstance = (VMSnapshotImpl *)j9mem_allocate_memory(sizeof(VMSnapshotImpl), J9MEM_CATEGORY_CLASSES);
	if (NULL != jvmInstance) {
		new(jvmInstance) VMSnapshotImpl(portLibrary, ramCache);
	}

	return jvmInstance;
}

bool
VMSnapshotImpl::setupWarmRun(void)
{
	bool success = true;
	if (!readImageFromFile()) {
		success = false;
		goto done;
	}

	if (!initializeMonitor()) {
		success = false;
		goto done;
	}

	if (!restoreJ9JavaVMStructures()) {
		success = false;
		goto done;
	}

done:
	return success;
}

bool
VMSnapshotImpl::setupColdRun(void)
{
	bool success = true;
	if (NULL == allocateImageMemory(INITIAL_IMAGE_SIZE)) {
		success = false;
		goto done;
	}

	if (NULL == initializeHeap()) {
		success = false;
		goto done;
	}

	if (!initializeMonitor()) {
		success = false;
		goto done;
	}
	
	if (!initializeInvalidITable()) {
		success = false;
		goto done;
	}

done:
	return success;
}

void *
VMSnapshotImpl::allocateImageMemory(UDATA size)
{
	PORT_ACCESS_FROM_PORT(_portLibrary);
	UDATA pageSize = j9vmem_supported_page_sizes()[0];
	void *imageAddress = j9mem_allocate_memory(size + pageSize, J9MEM_CATEGORY_CLASSES);
	if (NULL == imageAddress) {
		return NULL;
	}

	_snapshotHeader = (SnapshotHeader *) ROUND_UP_TO_POWEROF2((UDATA)imageAddress, pageSize);
	_snapshotHeader->imageAddress = (uintptr_t)imageAddress;
	_snapshotHeader->imageAlignedAddress = (uintptr_t)_snapshotHeader;
	_snapshotHeader->imageSize = size;

	return _snapshotHeader;
}

/* TODO: Currently reallocating image memory is broken since all references to memory inside of heap will fail (i.e. vm->classLoadingPool) */
/* TODO: Better approach is to be able to deal with multiple suballocator regions */
void *
VMSnapshotImpl::reallocateImageMemory(UDATA size)
{
	Assert_VM_unreachable();
	return NULL;
}

void * 
VMSnapshotImpl::initializeHeap(void)
{
	PORT_ACCESS_FROM_PORT(_portLibrary);
	UDATA pageSize = j9vmem_supported_page_sizes()[0];
	
	_heap = j9heap_create((J9Heap *)(_snapshotHeader + 1), ROUND_DOWN_TO_POWEROF2(VMSnapshotImpl::INITIAL_IMAGE_SIZE - sizeof(_snapshotHeader), pageSize), 0);
	if (NULL == _heap) {
		return NULL;
	}

	return _heap;
}

void *
VMSnapshotImpl::subAllocateMemory(uintptr_t byteAmount)
{
	Trc_VM_SubAllocateImageMemory_Entry(_snapshotHeader, byteAmount);

	omrthread_monitor_enter(_vmSnapshotImplMonitor);

	OMRPortLibrary *portLibrary = VMSNAPSHOTIMPL_OMRPORT_FROM_VMSNAPSHOTIMPLPORT(_vmSnapshotImplPortLibrary);
	void *memStart = portLibrary->heap_allocate(portLibrary, _heap, byteAmount);	
	/* image memory is not large enough and needs to be reallocated */
	if (NULL == memStart) {
		reallocateImageMemory(_snapshotHeader->imageSize * 2 + byteAmount); /* guarantees size of heap will accomodate byteamount */
		memStart = portLibrary->heap_allocate(portLibrary, _heap, byteAmount);
	}

	omrthread_monitor_exit(_vmSnapshotImplMonitor);

	Trc_VM_SubAllocateImageMemory_Exit(memStart);

	return memStart;
}

void*
VMSnapshotImpl::reallocateMemory(void *address, uintptr_t byteAmount)
{
	omrthread_monitor_enter(_vmSnapshotImplMonitor);

	OMRPortLibrary *portLibrary = VMSNAPSHOTIMPL_OMRPORT_FROM_VMSNAPSHOTIMPLPORT(_vmSnapshotImplPortLibrary);
	void *memStart = portLibrary->heap_reallocate(portLibrary, _heap, address, byteAmount);
	/* image memory is not large enough and needs to be reallocated */
	if (NULL == memStart) {
		reallocateImageMemory(_snapshotHeader->imageSize * 2 + byteAmount); /* guarantees size of heap will accomodate byteamount */
		memStart = portLibrary->heap_reallocate(portLibrary, _heap, address, byteAmount);
	}

	omrthread_monitor_exit(_vmSnapshotImplMonitor);

	return memStart;
}

void
VMSnapshotImpl::freeSubAllocatedMemory(void *address)
{
	omrthread_monitor_enter(_vmSnapshotImplMonitor);

	OMRPortLibrary *portLibrary = VMSNAPSHOTIMPL_OMRPORT_FROM_VMSNAPSHOTIMPLPORT(_vmSnapshotImplPortLibrary);
	portLibrary->heap_free(portLibrary, _heap, address);

	omrthread_monitor_exit(_vmSnapshotImplMonitor);
}

void
VMSnapshotImpl::saveClassLoaderBlocks(void)
{
	_snapshotHeader->savedJavaVMStructs.classLoaderBlocks = _vm->classLoaderBlocks;
	_snapshotHeader->savedJavaVMStructs.systemClassLoader = _vm->systemClassLoader;
	_snapshotHeader->savedJavaVMStructs.extensionClassLoader = _vm->extensionClassLoader;
	_snapshotHeader->savedJavaVMStructs.applicationClassLoader = _vm->applicationClassLoader;
}

void
VMSnapshotImpl::restoreClassLoaderBlocks(void)
{
	/* TRIGGER_J9HOOK_VM_CLASS_LOADER_CREATED is currently unused, if this changes we
	 * can not simply restore the classloaders
	 */
	_vm->classLoaderBlocks = _snapshotHeader->savedJavaVMStructs.classLoaderBlocks;
	_vm->systemClassLoader = _snapshotHeader->savedJavaVMStructs.systemClassLoader;
	_vm->extensionClassLoader = _snapshotHeader->savedJavaVMStructs.extensionClassLoader;
	_vm->applicationClassLoader = _snapshotHeader->savedJavaVMStructs.applicationClassLoader;
}

bool
VMSnapshotImpl::isImmortalClassLoader(J9ClassLoader *classLoader)
{
	if ((classLoader == _vm->applicationClassLoader)
	|| (classLoader == _vm->systemClassLoader)
	|| (classLoader == _vm->extensionClassLoader)
	|| (NULL == classLoader)
	) {
		return true;
	}

	return false;
}

void
printAllSegments(J9MemorySegmentList *segmentList, J9JavaVM *_vm)
{
	J9MemorySegment *currentSegment = segmentList->nextSegment;
	int i = 0;

	if (_vm->classMemorySegments == segmentList) {
		printf("class memory segments ");
	}
	printf("Segment %p Total segments ------------ \n", segmentList);

	while (NULL != currentSegment) {
		printf("segment=%p heapBase=%p size=%d\n", currentSegment, currentSegment->heapBase, (int)currentSegment->size);
		i++;
		currentSegment = currentSegment->nextSegment;
	}
	printf("%p Total segments %d -------------\n", segmentList, (int) i);
}

J9MemorySegmentList*
VMSnapshotImpl::copyUnPersistedMemorySegmentsToNewList(J9MemorySegmentList *oldMemorySegmentList)
{
	J9MemorySegment *currentSegment = oldMemorySegmentList->nextSegment;
	J9MemorySegmentList *newMemorySegmentList = _vm->internalVMFunctions->allocateMemorySegmentList(_vm, 10, OMRMEM_CATEGORY_VM);

	if (NULL == newMemorySegmentList) {
		_vm->internalVMFunctions->setNativeOutOfMemoryError(currentVMThread(_vm), 0 ,0);
		goto done;
	}

	/* copy all the persisted memory segments into the new list */
	while (NULL != currentSegment) {
		J9MemorySegment *nextSegment = currentSegment->nextSegment;

		if (!IS_SEGMENT_PERSISTED(currentSegment) || !isImmortalClassLoader(currentSegment->classLoader)) {
			J9MemorySegment *newSegment = allocateMemorySegmentListEntry(newMemorySegmentList);

			newSegment->type = currentSegment->type;
			newSegment->size = currentSegment->size;
			newSegment->baseAddress = currentSegment->baseAddress;
			newSegment->heapBase = currentSegment->heapBase;
			newSegment->heapTop = currentSegment->heapTop;
			newSegment->heapAlloc = currentSegment->heapAlloc;

			newMemorySegmentList->totalSegmentSize += newSegment->size;
			if (0 != (newMemorySegmentList->flags & MEMORY_SEGMENT_LIST_FLAG_SORT)) {
				avl_insert(&newMemorySegmentList->avlTreeData, (J9AVLTreeNode *) newSegment);
			}

			/* remove segment from old list */
			if (0 != (oldMemorySegmentList->flags & MEMORY_SEGMENT_LIST_FLAG_SORT)) {
				avl_delete(&oldMemorySegmentList->avlTreeData, (J9AVLTreeNode *) currentSegment);
			}
			J9_MEMORY_SEGMENT_LINEAR_LINKED_LIST_REMOVE(oldMemorySegmentList->nextSegment, currentSegment);
			pool_removeElement(oldMemorySegmentList->segmentPool, currentSegment);
		}
		currentSegment = nextSegment;
	}

done:
	return newMemorySegmentList;
}

void
VMSnapshotImpl::saveMemorySegments(void)
{
	/* The approach here is to copy unpersisted segments to a new list, and free those.
	 * The persisted segments will be stored in the image.
	 */
	_snapshotHeader->savedJavaVMStructs.classMemorySegments = _vm->classMemorySegments;
	_vm->classMemorySegments = copyUnPersistedMemorySegmentsToNewList(_vm->classMemorySegments);

	_snapshotHeader->savedJavaVMStructs.memorySegments = _vm->memorySegments;
	_vm->memorySegments = copyUnPersistedMemorySegmentsToNewList(_vm->memorySegments);

	return;
}

void
VMSnapshotImpl::restoreMemorySegments(void)
{
	_vm->memorySegments = _snapshotHeader->savedJavaVMStructs.memorySegments;
	_vm->classMemorySegments = _snapshotHeader->savedJavaVMStructs.classMemorySegments;
}

void
VMSnapshotImpl::savePrimitiveAndArrayClasses(void)
{
	J9Class **saveClasses = &_snapshotHeader->savedJavaVMStructs.voidReflectClass;
	J9Class **liveClasses = &_vm->voidReflectClass;

	for (int i = 0; i < 17; i++) {
		saveClasses[i] = liveClasses[i];
	}
}

bool
VMSnapshotImpl::restorePrimitiveAndArrayClasses(void)
{
	J9Class **saveClasses = &_snapshotHeader->savedJavaVMStructs.voidReflectClass;
	J9Class **liveClasses = &_vm->voidReflectClass;
	bool rc = true;

	for (int i = 0; i < 17; i++) {
		liveClasses[i] = saveClasses[i];
	}

	return rc;
}




void
VMSnapshotImpl::fixupVMStructures(void)
{
	UDATA omrRuntimeOffset = ROUND_UP_TO_POWEROF2(sizeof(J9JavaVM), 8);
	UDATA omrVMOffset = ROUND_UP_TO_POWEROF2(omrRuntimeOffset + sizeof(OMR_Runtime), 8);
	UDATA vmAllocationSize = omrVMOffset + sizeof(OMR_VM);
	memset(_vm, 0, vmAllocationSize);
}

void
VMSnapshotImpl::fixupClassLoaders(void)
{
	pool_state classLoaderWalkState = {0};
	J9ClassLoader *currentClassLoader = (J9ClassLoader *) pool_startDo(_vm->classLoaderBlocks, &classLoaderWalkState);
	while (NULL != currentClassLoader) {
		currentClassLoader->classPathEntries = NULL;
		currentClassLoader->sharedLibraries = NULL;
		currentClassLoader->librariesHead = NULL;
		currentClassLoader->librariesTail = NULL;
		currentClassLoader->classLoaderObject = NULL;
		currentClassLoader->unloadLink = NULL;
		currentClassLoader->gcLinkNext = NULL;
		currentClassLoader->gcLinkPrevious = NULL;
		currentClassLoader->gcFlags = 0;
		currentClassLoader->gcRememberedSet = 0;
		currentClassLoader->gcThreadNotification = NULL;
		currentClassLoader->jniIDs = NULL;
		currentClassLoader->jniRedirectionBlocks = NULL;
		currentClassLoader->gcRememberedSet = 0;
		currentClassLoader->jitMetaDataList = NULL;
		currentClassLoader->flags &= ~J9CLASSLOADER_CLASSPATH_SET; /* TODO in the future persist these as well */

		currentClassLoader = (J9ClassLoader *) pool_nextDo(&classLoaderWalkState);
	}
}

void
VMSnapshotImpl::removeUnpersistedClassLoaders(void)
{
	/* wont need this once we support all classloaders */
	pool_state classLoaderWalkState = {0};
	UDATA numOfClassLoaders = pool_numElements(_vm->classLoaderBlocks);
	U_8 buf[CLASS_LOADER_REMOVE_COUNT * sizeof(J9ClassLoader*)];
	J9ClassLoader **removeLoaders = (J9ClassLoader **) buf;
	J9VMThread *vmThread = currentVMThread(_vm);
	UDATA count = 0;
	PORT_ACCESS_FROM_PORT(_portLibrary);

	if (CLASS_LOADER_REMOVE_COUNT < numOfClassLoaders) {

		removeLoaders = (J9ClassLoader **) j9mem_allocate_memory(numOfClassLoaders * sizeof(J9ClassLoader*), J9MEM_CATEGORY_CLASSES);
		if (NULL == removeLoaders) {
			/* TODO error handling for fixups */
			Assert_VM_unreachable();
		}
	}

	J9ClassLoader *classloader = (J9ClassLoader *) pool_startDo(_vm->classLoaderBlocks, &classLoaderWalkState);
	while (NULL != classloader) {
		if (!isImmortalClassLoader(classloader)) {
			removeLoaders[count] = classloader;
			count++;
		}
		classloader = (J9ClassLoader *) pool_nextDo(&classLoaderWalkState);
	}

	for (UDATA i = 0; i < count; i++) {
		freeClassLoader(removeLoaders[i], _vm, vmThread, FALSE);
	}

	if ((J9ClassLoader **)buf != removeLoaders) {
		j9mem_free_memory(removeLoaders);
	}
}


void
VMSnapshotImpl::fixupClasses(void)
{
	pool_state classLoaderWalkState = {0};
	J9ClassLoader *classloader = (J9ClassLoader *) pool_startDo(_vm->classLoaderBlocks, &classLoaderWalkState);
	while (NULL != classloader) {
		J9ClassWalkState walkState = {0};
		J9Class *currentClass = allLiveClassesStartDo(&walkState, _vm, classloader);

		while (NULL != currentClass) {
			J9ROMClass *romClass = currentClass->romClass;

			if (J9ROMCLASS_IS_ARRAY(romClass)) {
				fixupArrayClass((J9ArrayClass *) currentClass);
			} else {
				fixupClass(currentClass);
			}

			/* Fixup the last ITable */
			currentClass->lastITable = (J9ITable *) currentClass->iTable;
			if (NULL == currentClass->lastITable) {
				currentClass->lastITable = VMSnapshotImpl::getInvalidITable();
			}

			currentClass = allLiveClassesNextDo(&walkState);
		}
		allLiveClassesEndDo(&walkState);

		classloader = (J9ClassLoader *) pool_nextDo(&classLoaderWalkState);
	}


}

void
VMSnapshotImpl::fixupClass(J9Class *clazz)
{
	clazz->classObject = NULL;
	fixupMethodRunAddresses(clazz);
	fixupConstantPool(clazz);
	clazz->initializeStatus = J9ClassInitNotInitialized;
	clazz->jniIDs = NULL;
	clazz->replacedClass = NULL;
	clazz->gcLink = NULL;
	clazz->jitMetaDataList = NULL;

	UDATA totalStaticSlots = totalStaticSlotsForClass(clazz->romClass);
	memset(clazz->ramStatics, 0, totalStaticSlots * sizeof(UDATA));

	UDATA i;
	if (NULL != clazz->staticSplitMethodTable) {
		for (i = 0; i < clazz->romClass->staticSplitMethodRefCount; i++) {
			clazz->staticSplitMethodTable[i] = (J9Method*)_vm->initialMethods.initialStaticMethod;
		}
	}
	if (NULL != clazz->specialSplitMethodTable) {
		for (i = 0; i < clazz->romClass->specialSplitMethodRefCount; i++) {
			clazz->specialSplitMethodTable[i] = (J9Method*)_vm->initialMethods.initialSpecialMethod;
		}
	}
	if (NULL != clazz->callSites) {
		memset(clazz->callSites, 0, sizeof(UDATA) * clazz->romClass->callSiteCount);
	}

	if (NULL != clazz->methodTypes) {
		memset(clazz->methodTypes, 0, sizeof(UDATA) * clazz->romClass->methodTypeCount);
	}

	if (NULL != clazz->varHandleMethodTypes) {
		memset(clazz->varHandleMethodTypes, 0, sizeof(UDATA) * clazz->romClass->varHandleMethodTypeCount);
	}
}

void
VMSnapshotImpl::fixupJITVtable(J9Class *ramClass)
{
	J9VMThread *vmThread = currentVMThread(_vm);
	J9JITConfig *jitConfig = _vm->jitConfig;
	J9VTableHeader *vTableAddress = J9VTABLE_HEADER_FROM_RAM_CLASS(ramClass);
	if (jitConfig != NULL) {
		UDATA *vTableWriteCursor = JIT_VTABLE_START_ADDRESS(ramClass);
		UDATA vTableWriteIndex = vTableAddress->size;
		J9Method **vTableReadCursor;
		if (vTableWriteIndex != 0) {
			if ((jitConfig->runtimeFlags & J9JIT_TOSS_CODE) != 0) {
				vTableWriteCursor -= vTableWriteIndex;
			} else {

				vTableReadCursor = J9VTABLE_FROM_HEADER(vTableAddress);
				for (; vTableWriteIndex > 0; vTableWriteIndex--) {
					J9Method *currentMethod = *vTableReadCursor;
					fillJITVTableSlot(vmThread, vTableWriteCursor, currentMethod);

					vTableReadCursor++;
					vTableWriteCursor--;
				}
			}
		}
	}
}

void
VMSnapshotImpl::fixupArrayClass(J9ArrayClass *clazz)
{
	clazz->classObject = NULL;
	fixupConstantPool((J9Class *) clazz);
	clazz->initializeStatus = J9ClassInitSucceeded;
	clazz->jniIDs = NULL;
	clazz->replacedClass = NULL;
	clazz->callSites = NULL;
	clazz->methodTypes = NULL;
	clazz->varHandleMethodTypes = NULL;
	clazz->gcLink = NULL;

	UDATA i;
	if (NULL != clazz->staticSplitMethodTable) {
		for (i = 0; i < clazz->romClass->staticSplitMethodRefCount; i++) {
			clazz->staticSplitMethodTable[i] = (J9Method*)_vm->initialMethods.initialStaticMethod;
		}
	}
	if (NULL != clazz->specialSplitMethodTable) {
		for (i = 0; i < clazz->romClass->specialSplitMethodRefCount; i++) {
			clazz->specialSplitMethodTable[i] = (J9Method*)_vm->initialMethods.initialSpecialMethod;
		}
	}
}

void
VMSnapshotImpl::fixupMethodRunAddresses(J9Class *ramClass)
{
	J9VMThread *vmThread = currentVMThread(_vm);
	J9ROMClass *romClass = ramClass->romClass;

	/* for all ramMethods call initializeMethodRunAdress for special methods */
	if (romClass->romMethodCount != 0) {
		UDATA i;
		UDATA count = romClass->romMethodCount;
		J9Method *ramMethod = ramClass->ramMethods;
		for (i = 0; i < count; i++) {
			initializeMethodRunAddressImpl(vmThread, ramMethod, FALSE);
			ramMethod++;
		}
	}
}

void
VMSnapshotImpl::fixupConstantPool(J9Class *ramClass)
{
	J9VMThread *vmThread = currentVMThread(_vm);
	J9ROMClass *romClass = ramClass->romClass;
	J9ConstantPool *ramCP = ((J9ConstantPool *) ramClass->ramConstantPool);
	J9ConstantPool *ramCPWithoutHeader = ramCP + 1;
	UDATA ramCPCount = romClass->ramConstantPoolCount;
	UDATA ramCPCountWithoutHeader = ramCPCount - 1;
	
	/* Zero the ramCP and initialize constant pool */
	if (ramCPCount != 0) {
		memset(ramCPWithoutHeader, 0, ramCPCountWithoutHeader * sizeof(J9RAMConstantPoolItem));
		internalRunPreInitInstructions(ramClass, vmThread);
	}
}

bool
VMSnapshotImpl::readImageFromFile(void)
{
	Trc_VM_ReadImageFromFile_Entry(_heap, _ramCache);

	OMRPORT_ACCESS_FROM_OMRPORT(&_portLibrary->omrPortLibrary);

	intptr_t fileDescriptor = omrfile_open(_ramCache, EsOpenRead | EsOpenWrite, 0444);
	if (-1 == fileDescriptor) {
		printf("falied to open ramCache file file=%s \n", _ramCache);
		return false;
	}

	/* Read image header size and location then mmap the rest of the image (heap) into memory */
	SnapshotHeader imageHeaderBuffer;
	if (-1 == omrfile_read(fileDescriptor, (void *)&imageHeaderBuffer, sizeof(SnapshotHeader))) {
		return false;
	}

	/* TODO: currently mmap uses sys/mman.h and MAP_FIXED. Once random memory loading is complete it will change to omr mmap without MAP_FIXED */
	_snapshotHeader = (SnapshotHeader *)mmap(
		(void *)imageHeaderBuffer.imageAlignedAddress,
		imageHeaderBuffer.imageSize,
		PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_FIXED, fileDescriptor, 0);
	if (-1 == (IDATA)_snapshotHeader) {
		return false;
	}
	if (imageHeaderBuffer.imageAlignedAddress != (UDATA) _snapshotHeader) {
		return false;
	}
	_heap = (J9Heap *)(_snapshotHeader + 1);
	_vm = _snapshotHeader->vm;
	_vm->ramStateFilePath = (char*) _ramCache;

	omrfile_close(fileDescriptor);

	Trc_VM_ReadImageFromFile_Exit();

	return true;
}

/**
 * Will be removed once the J9JavaVM struct is fully persisted
 */
void
VMSnapshotImpl::saveJ9JavaVMStructures(void)
{
	saveClassLoaderBlocks();
	saveMemorySegments();
	savePrimitiveAndArrayClasses();
	_snapshotHeader->vm = _vm;
}

/**
 * Will be removed once the J9JavaVM struct is fully persisted
 */
bool
VMSnapshotImpl::restoreJ9JavaVMStructures(void)
{
	bool success = true;
	fixupVMStructures();

	restoreClassLoaderBlocks();
	restoreMemorySegments();


	if (omrthread_monitor_init_with_name(&_vm->classMemorySegments->segmentMutex, 0, "VM class mem segment list")) {
		success = false;
	}

	if (omrthread_monitor_init_with_name(&_vm->memorySegments->segmentMutex, 0, "VM mem segment list")) {
		success = false;
	}

	if (!restorePrimitiveAndArrayClasses()) {
		success = false;
	}

	return success;
}


bool
VMSnapshotImpl::writeImageToFile(void)
{
	Trc_VM_WriteImageToFile_Entry(_heap, _ramCache);

	OMRPortLibrary *portLibrary = (OMRPortLibrary *) _portLibrary;
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	intptr_t fileDescriptor = omrfile_open(_ramCache, EsOpenCreate | EsOpenCreateAlways | EsOpenWrite, 0666);
	if (-1 == fileDescriptor) {
		return false;
	}

	if ((intptr_t)_snapshotHeader->imageSize != omrfile_write(fileDescriptor, (void *)_snapshotHeader, _snapshotHeader->imageSize)) {
		return false;
	}

	omrfile_close(fileDescriptor);

	Trc_VM_WriteImageToFile_Exit();

	return true;
}

void
VMSnapshotImpl::teardownImage(void)
{
	removeUnpersistedClassLoaders();
	/* TODO this is the place where we'll want to call the GC API to snapshot the heap */
	fixupClassLoaders();
	fixupClasses();
	saveJ9JavaVMStructures();
	writeImageToFile();
}

VMSnapshotImplPortLibrary *
setupVMSnapshotImplPortLibrary(J9PortLibrary *portLibrary)
{
	PORT_ACCESS_FROM_PORT(portLibrary);
	VMSnapshotImplPortLibrary *vmSnapshotImplPortLibrary = (VMSnapshotImplPortLibrary*)j9mem_allocate_memory(sizeof(VMSnapshotImplPortLibrary), OMRMEM_CATEGORY_PORT_LIBRARY);

	OMRPORT_ACCESS_FROM_J9PORT(portLibrary);
	memcpy(&(vmSnapshotImplPortLibrary->portLibrary), privateOmrPortLibrary, sizeof(OMRPortLibrary));
	vmSnapshotImplPortLibrary->portLibrary.mem_allocate_memory = image_mem_allocate_memory;
	vmSnapshotImplPortLibrary->portLibrary.mem_free_memory = image_mem_free_memory;
	vmSnapshotImplPortLibrary->vmSnapshotImpl = NULL;

	return vmSnapshotImplPortLibrary;
}


extern "C" void *
initializeVMSnapshotImpl(J9PortLibrary *portLibrary, BOOLEAN isSnapShotRun, const char* ramCache)
{
	VMSnapshotImplPortLibrary *vmSnapshotImplPortLibrary = setupVMSnapshotImplPortLibrary(portLibrary);
	VMSnapshotImpl *vmSnapshotImpl = VMSnapshotImpl::createInstance(portLibrary, ramCache);
	if (NULL == vmSnapshotImplPortLibrary || NULL == vmSnapshotImpl) {
		goto _error;
	}

	vmSnapshotImplPortLibrary->vmSnapshotImpl = vmSnapshotImpl;
	((VMSnapshotImpl *)vmSnapshotImplPortLibrary->vmSnapshotImpl)->setImagePortLib(vmSnapshotImplPortLibrary);

	if (isSnapShotRun && (!vmSnapshotImpl->setupColdRun())) {
		goto _error;
	}

	if(!isSnapShotRun && (!vmSnapshotImpl->setupWarmRun())) {
		goto _error;
	}

	return vmSnapshotImpl;

_error:
	shutdownVMSnapshotImpl(vmSnapshotImplPortLibrary);
	vmSnapshotImpl->destroyMonitor();
	return NULL;
}

extern "C" void
setupVMSnapshotImpl(void *vmSnapshotImpl, J9JavaVM* vm)
{
	((VMSnapshotImpl *)vmSnapshotImpl)->setJ9JavaVM(vm);
	vm->vmSnapshotImplPortLibrary = ((VMSnapshotImpl *)vmSnapshotImpl)->getVMSnapshotImplPortLibrary();
}

extern "C" J9JavaVM *
getJ9JavaVMFromVMSnapshotImpl(void *vmSnapshotImpl)
{
	return ((VMSnapshotImpl *)vmSnapshotImpl)->getJ9JavaVM();
}

extern "C" VMSnapshotImplPortLibrary *
getPortLibraryFromVMSnapshotImpl(void *vmSnapshotImpl)
{
	return ((VMSnapshotImpl *)vmSnapshotImpl)->getVMSnapshotImplPortLibrary();
}

extern "C" void
initializeImageClassLoaderObject(J9JavaVM *javaVM, J9ClassLoader *classLoader, j9object_t classLoaderObject)
{
	J9VMThread *vmThread = currentVMThread(javaVM);

	omrthread_monitor_enter(javaVM->classLoaderBlocksMutex);

	J9CLASSLOADER_SET_CLASSLOADEROBJECT(vmThread, classLoader, classLoaderObject);

	issueWriteBarrier();
	J9VMJAVALANGCLASSLOADER_SET_VMREF(vmThread, classLoaderObject, classLoader);
	
	omrthread_monitor_exit(javaVM->classLoaderBlocksMutex);
}

extern "C" void
initializeImageJ9Class(J9JavaVM *javaVM, J9Class *clazz)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)javaVM->vmSnapshotImplPortLibrary->vmSnapshotImpl;
	Assert_VM_notNull(vmSnapshotImpl);

	vmSnapshotImpl->fixupJITVtable(clazz);
}


extern "C" J9Class *
initializeImageClassObject(J9VMThread *vmThread, J9ClassLoader *classLoader, J9Class *clazz)
{
	J9JavaVM *javaVM = vmThread->javaVM;

	/* Allocate class object */
	J9Class *jlClass = J9VMJAVALANGCLASS_OR_NULL(javaVM);

	if (NULL == jlClass) {
		return NULL;
	}
	J9Class *lockClass = J9VMJAVALANGJ9VMINTERNALSCLASSINITIALIZATIONLOCK_OR_NULL(javaVM);

	j9object_t classObject = javaVM->memoryManagerFunctions->J9AllocateObject(vmThread, jlClass, J9_GC_ALLOCATE_OBJECT_TENURED | J9_GC_ALLOCATE_OBJECT_NON_INSTRUMENTABLE | J9_GC_ALLOCATE_OBJECT_HASHED);
	if (NULL == classObject) {
		setHeapOutOfMemoryError(vmThread);
		return NULL;
	}

	/* Allocate lock object if lock class already allocated */
	if (NULL != lockClass) {
		j9object_t lockObject;
		UDATA allocateFlags = J9_GC_ALLOCATE_OBJECT_NON_INSTRUMENTABLE;

		PUSH_OBJECT_IN_SPECIAL_FRAME(vmThread, (j9object_t)classObject);
		lockObject = javaVM->memoryManagerFunctions->J9AllocateObject(vmThread, lockClass, allocateFlags);
		classObject = POP_OBJECT_IN_SPECIAL_FRAME(vmThread);

		if (NULL == lockObject) {
			setHeapOutOfMemoryError(vmThread);
			return NULL;
		}
		J9VMJAVALANGJ9VMINTERNALSCLASSINITIALIZATIONLOCK_SET_THECLASS(vmThread, lockObject, classObject);
		J9VMJAVALANGCLASS_SET_INITIALIZATIONLOCK(vmThread, classObject, lockObject);
	}

	J9VMJAVALANGCLASS_SET_CLASSLOADER(vmThread, classObject, classLoader->classLoaderObject);
	J9VMJAVALANGCLASS_SET_VMREF(vmThread, classObject, clazz);
	J9STATIC_OBJECT_STORE(vmThread, clazz, (j9object_t*)&clazz->classObject, (j9object_t)classObject);

	return clazz;
}

extern "C" void
shutdownVMSnapshotImpl(VMSnapshotImplPortLibrary *vmSnapshotImplPortLibrary)
{
	if (NULL != vmSnapshotImplPortLibrary->vmSnapshotImpl) {
		PORT_ACCESS_FROM_PORT(((VMSnapshotImpl *)vmSnapshotImplPortLibrary->vmSnapshotImpl)->getJ9JavaVM()->portLibrary);
		Assert_VM_notNull(vmSnapshotImplPortLibrary);

		VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *) vmSnapshotImplPortLibrary->vmSnapshotImpl;

		if (NULL != vmSnapshotImpl) {
			vmSnapshotImpl->~VMSnapshotImpl();
			j9mem_free_memory(vmSnapshotImpl);
			/* TODO */
			//j9mem_free_memory(VMSNAPSHOTIMPLPORT_FROM_JAVAVM(javaVM));
		}
		j9mem_free_memory(vmSnapshotImplPortLibrary);
	}
}

extern "C" void
teardownVMSnapshotImpl(J9JavaVM *javaVM)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)javaVM->vmSnapshotImplPortLibrary->vmSnapshotImpl;
	Assert_VM_notNull(vmSnapshotImpl);

	if (IS_SNAPSHOT_RUN(javaVM)) {
		vmSnapshotImpl->teardownImage();
	} else {
		vmSnapshotImpl->saveMemorySegments();
	}
}

extern "C" void
storeInitialVMMethods(J9JavaVM* javaVM, J9Method* cInitialStaticMethod, J9Method* cInitialSpecialMethod, J9Method* cInitialVirtualMethod)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)javaVM->vmSnapshotImplPortLibrary->vmSnapshotImpl;
	Assert_VM_notNull(vmSnapshotImpl);

	vmSnapshotImpl->storeInitialMethods(cInitialStaticMethod, cInitialSpecialMethod, cInitialVirtualMethod);
}

extern "C" void
setInitialVMMethods(J9JavaVM* javaVM, J9Method** cInitialStaticMethod, J9Method** cInitialSpecialMethod, J9Method** cInitialVirtualMethod)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)javaVM->vmSnapshotImplPortLibrary->vmSnapshotImpl;
	Assert_VM_notNull(vmSnapshotImpl);

	vmSnapshotImpl->setInitialMethods(cInitialStaticMethod, cInitialSpecialMethod, cInitialVirtualMethod);
}

extern "C" void *
image_mem_allocate_memory(struct OMRPortLibrary *portLibrary, uintptr_t byteAmount, const char *callSite, uint32_t category)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)((VMSnapshotImplPortLibrary *)portLibrary)->vmSnapshotImpl;

	Assert_VM_notNull(vmSnapshotImpl);

	void *pointer = vmSnapshotImpl->subAllocateMemory(byteAmount);
	return pointer;
}

extern "C" void
image_mem_free_memory(struct OMRPortLibrary *portLibrary, void *memoryPointer)
{
	VMSnapshotImpl *vmSnapshotImpl = (VMSnapshotImpl *)((VMSnapshotImplPortLibrary *)portLibrary)->vmSnapshotImpl;
	Assert_VM_notNull(vmSnapshotImpl);

	vmSnapshotImpl->freeSubAllocatedMemory(memoryPointer);
}
