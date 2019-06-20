/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of thse Eclipse Public License 2.0 which accompanies this
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

#include "j9protos.h"
#include "j9consts.h"
#include "ut_j9bcverify.h"
#include "omrlinkedlist.h"

static J9ClassRelationship *findClassRelationship(J9VMThread *vmThread, J9ClassLoader *classLoader, U_8 *className, UDATA classNameLength);
static void removeClassRelationship(J9VMThread *vmThread, J9ClassLoader *classLoader, J9ClassRelationship *relationship);
static UDATA relationshipHashFn(void *key, void *userData);
static UDATA relationshipHashEqualFn(void *leftKey, void *rightKey, void *userData);

/**
 * Record a class relationship in the class relationships table.
 * 
 * Returns TRUE if successful and FALSE if an out of memory error occurs.
 */
IDATA
j9bcv_recordClassRelationship (J9VMThread *vmThread, J9ClassLoader *classLoader, U_8 *childName, UDATA childNameLength, U_8 *parentName, UDATA parentNameLength, IDATA *reasonCode)
{
    PORT_ACCESS_FROM_VMC(vmThread);
    J9JavaVM *vm = vmThread->javaVM;
    J9ClassRelationship *childEntry = NULL;
    J9ClassRelationshipNode *parentNode = NULL;
    J9ClassRelationship child = {0};
    IDATA recordResult = FALSE;
    *reasonCode = BCV_ERR_INSUFFICIENT_MEMORY;

    Trc_RTV_recordClassRelationship_Entry(vmThread, childNameLength, childName, parentNameLength, parentName);

    Assert_RTV_true((NULL != childName) && (NULL != parentName));

    /* Locate existing childEntry or add new entry to the hashtable */
    childEntry = findClassRelationship(vmThread, classLoader, childName, childNameLength);

    if (NULL == childEntry) {
        child.className = (U_8 *) j9mem_allocate_memory(childNameLength + 1, J9MEM_CATEGORY_CLASSES);

        if (NULL != child.className) {
            memcpy(child.className, childName, childNameLength);
            child.className[childNameLength] = '\0';
            child.classNameLength = childNameLength;

            childEntry = hashTableAdd(classLoader->classRelationshipsHashTable, &child);

            if (NULL == childEntry) {
                Trc_RTV_recordClassRelationship_EntryAllocationFailedChild(vmThread);
                j9mem_free_memory(child.className);
                goto recordDone;
            }
        } else {
            Trc_RTV_recordClassRelationship_EntryAllocationFailedChild(vmThread);
            goto recordDone;
        }
    }

    /* Add a parentNode to the child's linked list of parents */
    parentNode = (J9ClassRelationshipNode *) j9mem_allocate_memory(sizeof(J9ClassRelationshipNode), J9MEM_CATEGORY_CLASSES);

    if (NULL != parentNode) {
        memset(parentNode, 0, sizeof(J9ClassRelationshipNode));
        parentNode->className = (U_8 *) j9mem_allocate_memory(parentNameLength + 1, J9MEM_CATEGORY_CLASSES);

        if (NULL != parentNode->className) {
            memcpy(parentNode->className, parentName, parentNameLength);
            parentNode->className[parentNameLength] = '\0';
            parentNode->classNameLength = parentNameLength;
            J9_LINKED_LIST_ADD_LAST(childEntry->root, parentNode);
            Trc_RTV_recordClassRelationship_AllocatedEntry(vmThread, childEntry->classNameLength, childEntry->className, childEntry, parentNode->classNameLength, parentNode->className, parentNode);
        } else {
            Trc_RTV_recordClassRelationship_NodeAllocationFailedParent(vmThread);
            j9mem_free_memory(parentNode);
            goto recordDone;
        }
    } else {
        Trc_RTV_recordClassRelationship_NodeAllocationFailedParent(vmThread);
        goto recordDone;
    }

    recordResult = TRUE;
    *reasonCode = 0;

recordDone:
    Trc_RTV_recordClassRelationship_Exit(vmThread, recordResult);
    return recordResult;
}

/**
 * Validate each recorded relationship for a class.
 * 
 * Returns NULL if successful, or the parent class that fails validation if unsuccessful.
 */
J9Class *
j9bcv_validateClassRelationships (J9VMThread *vmThread, J9ClassLoader *classLoader, U_8 *childName, UDATA childNameLength, J9Class *childClass)
{
    J9Class *parentClass = NULL;
    J9ClassRelationship *childEntry = NULL;
    J9ClassRelationshipNode *parentNode = NULL;

    Trc_RTV_validateClassRelationships_Entry(vmThread, childNameLength, childName);
    Assert_RTV_true(NULL != childName);
    childEntry = findClassRelationship(vmThread, classLoader, childName, childNameLength);

    /* No relationships were recorded for the class, or its relationships have already been verified */
    if (NULL == childEntry) {
        goto validateDone;
    }

    parentNode = J9_LINKED_LIST_START_DO(childEntry->root);
    Assert_RTV_true(NULL != parentNode);

    while (NULL != parentNode) {
        /* Find the parent class in the loaded classes table */
        parentClass = vmThread->javaVM->internalVMFunctions->hashClassTableAt(classLoader, parentNode->className, parentNode->classNameLength);

        /* The parent class has not been loaded */
        if (NULL == parentClass) {
            /* The class relationship is invalid since superinterfaces and superclasses should be loaded at this point */
            Trc_RTV_validateClassRelationships_ParentNotLoaded(vmThread, parentNode->classNameLength, parentNode->className, parentNode);
            goto validateDone;
        } else {
            /* parentClass should either be an interface, or is the same or superclass of the childClass */
            if (parentClass->romClass->modifiers & J9AccInterface) {
                J9ITable *interfaceElement = (J9ITable *)childClass->iTable;
                BOOLEAN foundParent = FALSE;

                Trc_RTV_validateClassRelationships_ParentIsInterface(vmThread, parentNode->classNameLength, parentNode->className, parentNode);

                /* Confirm that the parentClass exists in the childClass's iTable */
                while (NULL != interfaceElement) {
                    if ((interfaceElement->interfaceClass == parentClass ) || (isSameOrSuperInterfaceOf(parentClass, interfaceElement->interfaceClass))) {
                        foundParent = TRUE;
                        break;
                    }
                    interfaceElement = interfaceElement->next;                    
                }

                if (!foundParent) {
                    Trc_RTV_validateClassRelationships_ParentInterfaceNotFound(vmThread);
                    goto validateDone;
                }
            } else if (isSameOrSuperClassOf(parentClass, childClass)) {
                Trc_RTV_validateClassRelationships_ParentIsSuperClass(vmThread, parentNode->classNameLength, parentNode->className, parentNode);
            } else {
                /* The child and parent have an invalid relationship */
                Trc_RTV_validateClassRelationships_InvalidRelationship(vmThread, parentNode->classNameLength, parentNode->className);
                goto validateDone;
            }
        }
        parentNode = J9_LINKED_LIST_NEXT_DO(childEntry->root, parentNode);
    }

    /* Remove table entry and linked list and free memory for childEntry */
    removeClassRelationship(vmThread, classLoader, childEntry);
    parentClass = NULL;

validateDone:
    Trc_RTV_validateClassRelationships_Exit(vmThread, parentClass);
    return parentClass;
}

/**
 * Find the class relationship table entry for a particular class.
 * 
 * Returns the found J9ClassRelationship, or NULL if it is not found.
 */
static J9ClassRelationship *
findClassRelationship(J9VMThread *vmThread, J9ClassLoader *classLoader, U_8 *className, UDATA classNameLength)
{
    J9ClassRelationship *classEntry = NULL;
    J9JavaVM *vm = vmThread->javaVM;

    Trc_RTV_findClassRelationship_Entry(vmThread, classNameLength, className);

    if (NULL != classLoader->classRelationshipsHashTable) {
        J9ClassRelationship exemplar = {0};
        exemplar.className = className;
        exemplar.classNameLength = classNameLength;
        classEntry = hashTableFind(classLoader->classRelationshipsHashTable, &exemplar);
    }

    Trc_RTV_findClassRelationship_Exit(vmThread, classEntry);
    return classEntry;
}

/**
 * Remove a class relationship table entry and its linked list of parent nodes, freeing allocated memory.
 */
static void
removeClassRelationship(J9VMThread *vmThread, J9ClassLoader *classLoader, J9ClassRelationship *relationship)
{
    PORT_ACCESS_FROM_VMC(vmThread);
    J9ClassRelationshipNode *parentNode = NULL;

    Trc_RTV_removeClassRelationship_Entry(vmThread, relationship->classNameLength, relationship->className);

    while (NULL != relationship->root) {
        parentNode = relationship->root;
        Trc_RTV_removeClassRelationship_Parent(vmThread, parentNode->classNameLength, parentNode->className);
        J9_LINKED_LIST_REMOVE(relationship->root, parentNode);
        j9mem_free_memory(parentNode->className);
        j9mem_free_memory(parentNode);
    }

    j9mem_free_memory(relationship->className);

    Assert_RTV_true(0 == hashTableRemove(classLoader->classRelationshipsHashTable, relationship));

    Trc_RTV_removeClassRelationship_Exit(vmThread);
    return;
}

/**
 * Allocates new hash table to store class relationship entries.
 * 
 * Returns the new J9HashTable.
 */
J9HashTable *
j9bcv_hashClassRelationshipTableNew (J9JavaVM *vm)
{
    return hashTableNew(OMRPORT_FROM_J9PORT(vm->portLibrary), J9_GET_CALLSITE(), 256, sizeof(J9ClassRelationship), sizeof(char *), 0, J9MEM_CATEGORY_CLASSES, relationshipHashFn, relationshipHashEqualFn, NULL, vm);
}

/**
 * Frees memory for each J9ClassRelationship table entry, J9ClassRelationshipNode, 
 * and the classRelationships hash table itself.
 */
void
j9bcv_hashClassRelationshipTableFree (J9VMThread *vmThread, J9ClassLoader *classLoader)
{
    J9HashTableState hashTableState = {0};
    J9HashTable *classRelationshipsHashTable = classLoader->classRelationshipsHashTable;
    J9ClassRelationship *relationshipEntry = (J9ClassRelationship *)hashTableStartDo(classRelationshipsHashTable, &hashTableState);

    while (NULL != relationshipEntry) {
        removeClassRelationship(vmThread, classLoader, relationshipEntry);
        relationshipEntry = (J9ClassRelationship *)hashTableNextDo(&hashTableState);
    }

    if (NULL != classRelationshipsHashTable) {
        hashTableFree(classRelationshipsHashTable);
        classRelationshipsHashTable = NULL;
    }

    return;
}

static UDATA 
relationshipHashFn(void *key, void *userData)
{
    J9ClassRelationship *relationshipKey = key;
    J9JavaVM *vm = userData;

    UDATA utf8HashClass = vm->internalVMFunctions->computeHashForUTF8(relationshipKey->className, relationshipKey->classNameLength);

    return utf8HashClass;
}

static UDATA  
relationshipHashEqualFn(void *leftKey, void *rightKey, void *userData)
{
    J9ClassRelationship *left_relationshipKey = leftKey;
    J9ClassRelationship *right_relationshipKey = rightKey;

    UDATA classNameEqual = J9UTF8_DATA_EQUALS(left_relationshipKey->className, left_relationshipKey->classNameLength, right_relationshipKey->className, right_relationshipKey->classNameLength);

    return classNameEqual;
}
