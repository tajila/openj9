/*******************************************************************************
 * Copyright (c) 2001, 2019 IBM Corp. and others
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
/*
* JVMImage.hpp
*/

#ifndef JVMIMAGE_HPP_
#define JVMIMAGE_HPP_

#include "j9.h"
#include "jvmimageport.h"
#include "j9comp.h"
#include "j9protos.h"
#include "ut_j9vm.h"
#include "objhelp.h"
#include "vm_api.h"

class JVMImage
{
	/*
	 * Data Members
	 */
private:
	J9JavaVM *_vm;
	J9PortLibrary *_portLibrary;
	JVMImageHeader *_jvmImageHeader;
	J9Heap *_heap;
	J9ITable *_invalidITable;
	const char *_ramCache;
	omrthread_monitor_t _jvmImageMonitor;
	JVMImagePortLibrary *_jvmImagePortLibrary;
protected:
public:
	static const UDATA INITIAL_IMAGE_SIZE;
	static const UDATA CLASS_LOADER_REMOVE_COUNT;
	
	/*
	 * Function Members
	 */
private:
	bool initializeMonitor(void);
	bool initializeInvalidITable(void);

	/* Image Heap initialization and allocation functions */
	void* allocateImageMemory(UDATA size);
	void* reallocateImageMemory(UDATA size); /* TODO: Function will be used once random memory loading is allowed. Function not used currently */
	void* initializeHeap(void);

	bool allocateImageTableHeaders(void);
	void* allocateTable(ImageTableHeader *table, uintptr_t tableSize);
	void* reallocateTable(ImageTableHeader *table, uintptr_t tableSize);

	bool readImageFromFile(void);
	bool writeImageToFile(void);

	/* Fixup functions called during teardown sequence */
	void fixupClassLoaders(void);
	void fixupClasses(void);
	void fixupClass(J9Class *clazz);
	void fixupArrayClass(J9ArrayClass *clazz);
	void fixupMethodRunAddresses(J9Class *ramClass);
	void fixupConstantPool(J9Class *ramClass);
	void fixupClassPathEntries(void);
	void removeUnpersistedClassLoaders(void);
	void saveJ9JavaVMStructures(void);
	bool restorePrimitiveAndArrayClasses(void);
	bool restoreJ9JavaVMStructures(void);
	void setPrimitiveAndArrayClasses(J9JavaVM *vm);

protected:
	void *operator new(size_t size, void *memoryPointer) { return memoryPointer; }
public:
	JVMImage(J9PortLibrary *portLibrary, const char* ramCache);
	~JVMImage();

	static JVMImage* createInstance(J9PortLibrary *portLibrary, const char* ramCache);

	void setJ9JavaVM(J9JavaVM *vm);
	void setImagePortLib(JVMImagePortLibrary *jvmImagePortLibrary);
	J9JavaVM* getJ9JavaVM();

	void setClassLoaderBlocks(J9JavaVM *vm);
	J9Pool* getClassLoaderBlocks();
	void setClassMemorySegments(J9JavaVM *vm);
	J9MemorySegmentList* getClassMemorySegments();
	void setMemorySegments(J9JavaVM *vm);
	J9MemorySegmentList* getMemorySegments();

	ImageRC setupWarmRun();
	ImageRC setupColdRun();

	void fixupJITVtable(J9Class *ramClass);
	void fixupVMStructures(void);
	void teardownImage(void);

	/* Suballocator functions */
	void* subAllocateMemory(uintptr_t byteAmount);
	void* reallocateMemory(void *address, uintptr_t byteAmount);
	void freeSubAllocatedMemory(void *memStart);

	UDATA registerEntryInTable(ImageTableHeader *table, UDATA entry);
	void deregisterEntryInTable(ImageTableHeader *table, UDATA entry);
	void* retrieveEntryInTable(ImageTableHeader *table, UDATA entry);

	void destroyMonitor(void);

	/* TODO: currently the three major class loaders have their own accessors/mutators. This could be ported to hash table once user defined class works */
	void setClassLoader(J9ClassLoader *classLoader, uint32_t classLoaderCategory);
	J9ClassLoader* getSystemClassLoader(void) { return WSRP_GET(_jvmImageHeader->systemClassLoader, J9ClassLoader*); }
	J9ClassLoader* getApplicationClassLoader(void) { return WSRP_GET(_jvmImageHeader->appClassLoader, J9ClassLoader*); }
	J9ClassLoader* getExtensionClassLoader(void) { return WSRP_GET(_jvmImageHeader->extensionClassLoader, J9ClassLoader*); }
	
	/* Accessors for tables */
	ImageTableHeader* getClassLoaderTable(void) { return WSRP_GET(_jvmImageHeader->classLoaderTable, ImageTableHeader*); }
	ImageTableHeader* getClassTable(void) { return WSRP_GET(_jvmImageHeader->classTable, ImageTableHeader*); }
	ImageTableHeader* getClassPathEntryTable(void) { return WSRP_GET(_jvmImageHeader->classPathEntryTable, ImageTableHeader*); }

	/* VM Initial Methods accessors/mutators */
	void storeInitialMethods(J9Method *cInitialStaticMethod, J9Method *cInitialSpecialMethod, J9Method *cInitialVirtualMethod);
	void setInitialMethods(J9Method **cInitialStaticMethod, J9Method **cInitialSpecialMethod, J9Method **cInitialVirtualMethod);

	J9ITable* getInvalidITable(void) { return _invalidITable; }
};

#endif /* JVMIMAGE_H_ */
