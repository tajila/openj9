/*******************************************************************************
 * Copyright (c) 1991, 2019 IBM Corp. and others
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

#ifndef snapshotfileformat_h
#define snapshotfileformat_h

/**
 * TODO: This will not be needed once the J9JavaVM
 * structure is properly persisted
 */
typedef struct SavedJ9JavaVMStructures {
	J9Pool *classLoaderBlocks;
	J9MemorySegmentList *classMemorySegments;
	J9MemorySegmentList *memorySegments;
	J9Class *voidReflectClass;
	J9Class *booleanReflectClass;
	J9Class *charReflectClass;
	J9Class *floatReflectClass;
	J9Class *doubleReflectClass;
	J9Class *byteReflectClass;
	J9Class *shortReflectClass;
	J9Class *intReflectClass;
	J9Class *longReflectClass;
	J9Class *booleanArrayClass;
	J9Class *charArrayClass;
	J9Class *floatArrayClass;
	J9Class *doubleArrayClass;
	J9Class *byteArrayClass;
	J9Class *shortArrayClass;
	J9Class *intArrayClass;
	J9Class *longArrayClass;
	J9ClassLoader *systemClassLoader;
	J9ClassLoader *extensionClassLoader;
	J9ClassLoader *applicationClassLoader;
} SavedJ9JavaVMStructures;

/*
 * Struct containing data about image heap and quick access variables
 *
 * Allows us to dump this struct into file and reload easier
 * Allocated space for image data is longer than sizeof(JVMImageHeader) and includes heap. see @ref JVMImage::allocateImageMemory
 * All quick access variables are J9WSRP to allow reallocation in future. TODO: Allow heap reallocation 
 */
typedef struct SnapshotHeader {
	UDATA imageSize; /* image size in bytes */
	J9JavaVM *vm;
	SavedJ9JavaVMStructures savedJavaVMStructs;
	uintptr_t imageAddress;
	uintptr_t imageAlignedAddress; /* TODO: Will be removed once PAGE alignment is not needed anymore */
	/* TODO: only three main class loaders stored for prototype and quick access. Need to allow user defined classloaders */
	J9WSRP systemClassLoader;
	J9WSRP appClassLoader;
	J9WSRP extensionClassLoader;
	J9WSRP classLoaderTable;
	J9WSRP classTable;
	J9WSRP classPathEntryTable;
	J9WSRP cInitialStaticMethod;
	J9WSRP cInitialSpecialMethod;
	J9WSRP cInitialVirtualMethod;
} SnapshotHeader;


#endif /* snapshotfileformat_h */
