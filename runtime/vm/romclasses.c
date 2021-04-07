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

#include "j9.h"
#include "j9protos.h"
#include "j9consts.h"
#include "j9vmconstantpool.h"

#define TYPE_COUNT 9

#define OBJECT_ARRAY_NAME "[L"
#define BOOLEAN_ARRAY_NAME "[Z"
#define CHAR_ARRAY_NAME "[C"
#define FLOAT_ARRAY_NAME "[F"
#define DOUBLE_ARRAY_NAME "[D"
#define BYTE_ARRAY_NAME "[B"
#define SHORT_ARRAY_NAME "[S"
#define INT_ARRAY_NAME "[I"
#define LONG_ARRAY_NAME "[J"
#define VOID_CLASS_NAME "void"
#define BOOLEAN_CLASS_NAME "boolean"
#define CHAR_CLASS_NAME "char"
#define FLOAT_CLASS_NAME "float"
#define DOUBLE_CLASS_NAME "double"
#define BYTE_CLASS_NAME "byte"
#define SHORT_CLASS_NAME "short"
#define INT_CLASS_NAME "int"
#define LONG_CLASS_NAME "long"
#define OBJECT_CLASS_NAME "java/lang/Object"
#define CLONEABLE_CLASS_NAME "java/lang/Cloneable"
#define SERIALIZEABLE_CLASS_NAME "java/io/Serializable"

#define INLINE_UTF(name, value) struct { U_16 length; U_8 data[sizeof(value) - 1]; } name

typedef struct {
	J9ROMImageHeader header;
	J9ROMArrayClass objectArrayROMClass;
	J9ROMArrayClass booleanArrayROMClass;
	J9ROMArrayClass charArrayROMClass;
	J9ROMArrayClass floatArrayROMClass;
	J9ROMArrayClass doubleArrayROMClass;
	J9ROMArrayClass byteArrayROMClass;
	J9ROMArrayClass shortArrayROMClass;
	J9ROMArrayClass intArrayROMClass;
	J9ROMArrayClass longArrayROMClass;
	struct {
		J9SRP cloneable;
		J9SRP serializeable;
	} interfaceClasses;
	INLINE_UTF(objectArrayClassName, OBJECT_ARRAY_NAME);
	INLINE_UTF(booleanArrayClassName, BOOLEAN_ARRAY_NAME);
	INLINE_UTF(charArrayClassName, CHAR_ARRAY_NAME);
	INLINE_UTF(floatArrayClassName, FLOAT_ARRAY_NAME);
	INLINE_UTF(doubleArrayClassName, DOUBLE_ARRAY_NAME);
	INLINE_UTF(byteArrayClassName, BYTE_ARRAY_NAME);
	INLINE_UTF(shortArrayClassName, SHORT_ARRAY_NAME);
	INLINE_UTF(intArrayClassName, INT_ARRAY_NAME);
	INLINE_UTF(longArrayClassName, LONG_ARRAY_NAME);
	INLINE_UTF(objectClassName, OBJECT_CLASS_NAME);
	INLINE_UTF(cloneableClassName, CLONEABLE_CLASS_NAME);
	INLINE_UTF(serializeableClassName, SERIALIZEABLE_CLASS_NAME);
} J9ArrayROMClasses;

typedef struct {
	J9ROMImageHeader header;
	J9ROMReflectClass voidReflectROMClass;
	J9ROMReflectClass booleanReflectROMClass;
	J9ROMReflectClass charReflectROMClass;
	J9ROMReflectClass floatReflectROMClass;
	J9ROMReflectClass doubleReflectROMClass;
	J9ROMReflectClass byteReflectROMClass;
	J9ROMReflectClass shortReflectROMClass;
	J9ROMReflectClass intReflectROMClass;
	J9ROMReflectClass longReflectROMClass;
	INLINE_UTF(voidClassName, VOID_CLASS_NAME);
	INLINE_UTF(booleanClassName, BOOLEAN_CLASS_NAME);
	INLINE_UTF(charClassName, CHAR_CLASS_NAME);
	INLINE_UTF(floatClassName, FLOAT_CLASS_NAME);
	INLINE_UTF(doubleClassName, DOUBLE_CLASS_NAME);
	INLINE_UTF(byteClassName, BYTE_CLASS_NAME);
	INLINE_UTF(shortClassName, SHORT_CLASS_NAME);
	INLINE_UTF(intClassName, INT_CLASS_NAME);
	INLINE_UTF(longClassName, LONG_CLASS_NAME);
} J9BaseTypePrimitiveROMClasses;

static J9ArrayROMClasses arrayROMClasses;
static J9BaseTypePrimitiveROMClasses baseTypePrimitiveROMClasses;
static J9ArrayROMClasses *arrayROMClassesPtr = &arrayROMClasses;
static J9BaseTypePrimitiveROMClasses *baseTypePrimitiveROMClassesPtr = &baseTypePrimitiveROMClasses;

#define INIT_UTF8(field, str) \
		do { \
			field.length = sizeof(str) - 1; \
			memcpy(&field.data, str, sizeof(str) - 1); \
		} while(0)

/**
 * Initialize a ROM array class.
 *
 * @param romClass[in] the ROM class to initialize
 * @param className[in] the class name
 * @param arrayShape[in] the array shape
 * @param instanceShape[in] the instance shape
 * @param size[in] the size for the ROM class
 */
static void
initializeArrayROMClass(J9ROMArrayClass *romClass, J9UTF8 *className, U_32 arrayShape, U_32 instanceShape, U_32 size)
{
	romClass->romSize = size;
	NNSRP_SET(romClass->className, className);
	NNSRP_SET(romClass->superclassName, &arrayROMClassesPtr->objectClassName);
	romClass->modifiers = J9AccFinal | J9AccPublic | J9AccClassArray | J9AccAbstract;
	romClass->extraModifiers = J9AccClassCloneable | J9AccClassIsUnmodifiable;
	romClass->interfaceCount = sizeof(arrayROMClasses.interfaceClasses) / sizeof(J9SRP);
	NNSRP_SET(romClass->interfaces, &arrayROMClassesPtr->interfaceClasses);
	romClass->arrayShape = arrayShape;
	romClass->instanceShape = J9_OBJECT_HEADER_INDEXABLE | instanceShape;
}

/**
 * Initialize a base type ROM class.
 *
 * @param romClass[in] the ROM class to initialize
 * @param className[in] the class name
 * @param typeCode[in] the reflect type code
 * @param instanceShape[in] the instance shape
 * @param elementSize[in] the element size
 * @param size[in] the size for the ROM class
 */
static void
initializeBaseTypeROMClass(J9ROMReflectClass *romClass, J9UTF8 *className, U_32 typeCode, U_32 instanceShape, U_32 elementSize, U_32 size)
{
	romClass->romSize = size;
	NNSRP_SET(romClass->className, className);
	romClass->modifiers = J9AccFinal | J9AccPublic | J9AccClassInternalPrimitiveType | J9AccAbstract;
	romClass->extraModifiers = J9AccClassIsUnmodifiable;
	romClass->reflectTypeCode = typeCode;
	romClass->instanceShape = instanceShape;
	romClass->elementSize = elementSize;
}

void
initializeROMClasses(J9JavaVM *vm)
{
	UDATA referenceSize = J9JAVAVM_COMPRESS_OBJECT_REFERENCES(vm) ? sizeof(U_32) : sizeof(UDATA);

#if defined(J9VM_OPT_SNAPSHOTS)
	if (IS_SNAPSHOTTING_ENABLED(vm)) {
		if (IS_SNAPSHOT_RUN(vm)) {
			VMSNAPSHOTIMPLPORT_ACCESS_FROM_JAVAVM(vm);
			arrayROMClassesPtr = vmsnapshot_allocate_memory32(sizeof(J9ArrayROMClasses), J9MEM_CATEGORY_CLASSES);
			baseTypePrimitiveROMClassesPtr = vmsnapshot_allocate_memory32(sizeof(J9BaseTypePrimitiveROMClasses), J9MEM_CATEGORY_CLASSES);
		} else if (IS_RESTORE_RUN(vm)){
			arrayROMClassesPtr = (J9ArrayROMClasses *)vm->arrayROMClasses;
			baseTypePrimitiveROMClassesPtr = (J9BaseTypePrimitiveROMClasses *)vm->baseTypePrimitiveROMClasses;
		}
	} else
#endif /* defined(J9VM_OPT_SNAPSHOTS) */
	{
		memset(arrayROMClassesPtr, 0, sizeof(arrayROMClasses));
		memset(baseTypePrimitiveROMClassesPtr, 0, sizeof(baseTypePrimitiveROMClasses));
	}

	/* Initialize UTF data for the array classes */
	INIT_UTF8((*arrayROMClassesPtr).objectArrayClassName, OBJECT_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).booleanArrayClassName, BOOLEAN_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).charArrayClassName, CHAR_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).floatArrayClassName, FLOAT_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).doubleArrayClassName, DOUBLE_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).byteArrayClassName, BYTE_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).shortArrayClassName, SHORT_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).intArrayClassName, INT_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).longArrayClassName, LONG_ARRAY_NAME);
	INIT_UTF8((*arrayROMClassesPtr).objectClassName, OBJECT_CLASS_NAME);
	INIT_UTF8((*arrayROMClassesPtr).cloneableClassName, CLONEABLE_CLASS_NAME);
	INIT_UTF8((*arrayROMClassesPtr).serializeableClassName, SERIALIZEABLE_CLASS_NAME);
	/* Initialize the required fields in the array class ROM image header */
	(*arrayROMClassesPtr).header.romSize = sizeof(arrayROMClasses) - sizeof(arrayROMClasses.header);
	NNSRP_SET((*arrayROMClassesPtr).header.firstClass, &arrayROMClassesPtr->objectArrayROMClass);
	/* Set up the SRPs in the interface array */
	NNSRP_SET((*arrayROMClassesPtr).interfaceClasses.cloneable, &arrayROMClassesPtr->cloneableClassName);
	NNSRP_SET((*arrayROMClassesPtr).interfaceClasses.serializeable, &arrayROMClassesPtr->serializeableClassName);
	/* Initialize the array classes */
	initializeArrayROMClass(&arrayROMClassesPtr->objectArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->objectArrayClassName, (sizeof(U_32) == referenceSize) ? J9ArraySizeLongs : J9ArraySizeDoubles, OBJECT_HEADER_SHAPE_POINTERS, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->booleanArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->booleanArrayClassName, J9ArraySizeBytes, OBJECT_HEADER_SHAPE_BYTES, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->charArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->charArrayClassName, J9ArraySizeWords, OBJECT_HEADER_SHAPE_WORDS, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->floatArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->floatArrayClassName, J9ArraySizeLongs, OBJECT_HEADER_SHAPE_LONGS, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->doubleArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->doubleArrayClassName, J9ArraySizeDoubles, OBJECT_HEADER_SHAPE_DOUBLES, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->byteArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->byteArrayClassName, J9ArraySizeBytes, OBJECT_HEADER_SHAPE_BYTES, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->shortArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->shortArrayClassName, J9ArraySizeWords, OBJECT_HEADER_SHAPE_WORDS, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->intArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->intArrayClassName, J9ArraySizeLongs, OBJECT_HEADER_SHAPE_LONGS, sizeof(J9ROMArrayClass));
	initializeArrayROMClass(&arrayROMClassesPtr->longArrayROMClass, (J9UTF8*)&arrayROMClassesPtr->longArrayClassName, J9ArraySizeDoubles, OBJECT_HEADER_SHAPE_DOUBLES, sizeof(J9ArrayROMClasses) - offsetof(J9ArrayROMClasses, longArrayROMClass));
	vm->arrayROMClasses = &arrayROMClassesPtr->header;
	/* Initialize UTF data for the primitive classes */
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).voidClassName, VOID_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).booleanClassName, BOOLEAN_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).charClassName, CHAR_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).floatClassName, FLOAT_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).doubleClassName, DOUBLE_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).byteClassName, BYTE_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).shortClassName, SHORT_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).intClassName, INT_CLASS_NAME);
	INIT_UTF8((*baseTypePrimitiveROMClassesPtr).longClassName, LONG_CLASS_NAME);
	/* Initialize the required fields in the primitive class ROM image header */
	(*baseTypePrimitiveROMClassesPtr).header.romSize = sizeof(baseTypePrimitiveROMClasses) - sizeof(baseTypePrimitiveROMClasses.header);
	NNSRP_SET((*baseTypePrimitiveROMClassesPtr).header.firstClass, &baseTypePrimitiveROMClassesPtr->voidReflectROMClass);
	vm->baseTypePrimitiveROMClasses = (void*)baseTypePrimitiveROMClassesPtr;
	/* Initialize the primitive classes */
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->voidReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->voidClassName, J9VMCONSTANTPOOL_JAVALANGOBJECT, OBJECT_HEADER_SHAPE_MIXED, 0, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->booleanReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->booleanClassName, J9VMCONSTANTPOOL_JAVALANGBOOLEAN, OBJECT_HEADER_SHAPE_BYTES, 1, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->charReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->charClassName, J9VMCONSTANTPOOL_JAVALANGCHARACTER, OBJECT_HEADER_SHAPE_WORDS, 2, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->floatReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->floatClassName, J9VMCONSTANTPOOL_JAVALANGFLOAT, OBJECT_HEADER_SHAPE_LONGS, 4, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->doubleReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->doubleClassName, J9VMCONSTANTPOOL_JAVALANGDOUBLE, OBJECT_HEADER_SHAPE_DOUBLES, 8, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->byteReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->byteClassName, J9VMCONSTANTPOOL_JAVALANGBYTE, OBJECT_HEADER_SHAPE_BYTES, 1, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->shortReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->shortClassName, J9VMCONSTANTPOOL_JAVALANGSHORT, OBJECT_HEADER_SHAPE_WORDS, 2, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->intReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->intClassName, J9VMCONSTANTPOOL_JAVALANGINTEGER, OBJECT_HEADER_SHAPE_LONGS, 4, sizeof(J9ROMReflectClass));
	initializeBaseTypeROMClass(&baseTypePrimitiveROMClassesPtr->longReflectROMClass, (J9UTF8*)&baseTypePrimitiveROMClassesPtr->longClassName, J9VMCONSTANTPOOL_JAVALANGLONG, OBJECT_HEADER_SHAPE_DOUBLES, 8, sizeof(J9BaseTypePrimitiveROMClasses) - offsetof(J9BaseTypePrimitiveROMClasses, longReflectROMClass));
}

UDATA
internalCreateBaseTypePrimitiveAndArrayClasses(J9VMThread *currentThread)
{
	J9JavaVM *vm = currentThread->javaVM;
	J9ClassLoader *classLoader = vm->systemClassLoader;
	UDATA rc = 1;
	UDATA i = 0;
	J9ROMArrayClass *arrayROMClass = &arrayROMClassesPtr->booleanArrayROMClass;
	J9Class **arrayRAMClass = &vm->booleanArrayClass;
	J9ROMClass *primitiveROMClass = (J9ROMClass*)&baseTypePrimitiveROMClassesPtr->voidReflectROMClass;
	J9Class **primitiveRAMClass = &vm->voidReflectClass;

	/* create a segment for the base type ROM classes */
	if (NULL == romImageNewSegment(vm, &baseTypePrimitiveROMClassesPtr->header, TRUE, classLoader)) {
		goto done;
	}
	/* create a segment for the array ROM classes */
	if (NULL == romImageNewSegment(vm, &arrayROMClassesPtr->header, FALSE, classLoader)) {
		goto done;
	}
	for (i = 0; i < TYPE_COUNT; ++i) {
		J9Class *ramClass = NULL;
		omrthread_monitor_enter(vm->classTableMutex);
		ramClass = internalCreateRAMClassFromROMClass(
				currentThread, /* currentThread */
				classLoader, /* classLoader */
				primitiveROMClass, /* romClass */
				J9_FINDCLASS_FLAG_EXISTING_ONLY, /* options */
				NULL, /* elementClass */
				NULL, /* protectionDomain */
				NULL, /* methodRemapArray */
				J9_CP_INDEX_NONE, /* entryIndex */
				LOAD_LOCATION_UNKNOWN, /* locationType */
				NULL, /* classBeingRedefined */
				NULL /* hostClass */
			);
		if (NULL == ramClass) {
			goto done;
		}
		*primitiveRAMClass = ramClass;
		/* Slot 0 is object array which has no entry in the J9JavaVM and void which
		 * does not get an array class created for it.
		 */
		if (0 != i) {
			J9Class *ramArrayClass = internalCreateArrayClass(currentThread, arrayROMClass, ramClass);
			if (NULL == ramArrayClass) {
				goto done;
			}
			*arrayRAMClass = ramArrayClass;
			arrayROMClass += 1;
			arrayRAMClass += 1;
		}
		primitiveROMClass += 1;
		primitiveRAMClass += 1;
	}
	rc = 0;
done:
	return rc;
}
