/*******************************************************************************
 * Copyright (c) 2001, 2014 IBM Corp. and others
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

#include <jni.h>
#include "j9protos.h"
#include "omrlinkedlist.h"

/**
 * Check if a relationship has been recorded in the
 * classloader relationship table for the specified
 * child class and parent class.
 *
 * Class: org_openj9_test_classRelationshipVerifier_TestClassRelationshipVerifier
 * Method: isRelationshipRecorded
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 *
 * @param[in] env the JNI interface pointer
 * @param[in] childNameString the name of the child class
 * @param[in] parentNameString the name of the parent class
 */
JNIEXPORT jboolean JNICALL
Java_org_openj9_test_classRelationshipVerifier_TestClassRelationshipVerifier_isRelationshipRecorded(JNIEnv *env, jclass relationships, jstring childNameString, jstring parentNameString)
{
	jboolean foundRelationship = JNI_FALSE;
	J9VMThread *thread  = (J9VMThread *) env;
	J9ClassLoader * classLoader = thread->javaVM->bytecodeVerificationData->classLoader;
	const char * childNameUTFChars = (*env)->GetStringUTFChars(env, childNameString, NULL);
	const char * parentNameUTFChars = (*env)->GetStringUTFChars(env, parentNameString, NULL);

	if (NULL != classLoader->classRelationshipsHashTable) {
		U_8 *childName = (U_8 *) childNameUTFChars;
		J9ClassRelationship exemplar = {0};
		J9ClassRelationship *childEntry = NULL;

		exemplar.className = childName;
		exemplar.classNameLength = (UDATA) J9UTF8_LENGTH(childName);
		childEntry = hashTableFind(classLoader->classRelationshipsHashTable, &exemplar);

		if (NULL != childEntry) {
			U_8 *parentName = (U_8 *) parentNameUTFChars;
			UDATA parentNameLength = (UDATA) J9UTF8_LENGTH(parentName);
			J9ClassRelationshipNode *currentNode = J9_LINKED_LIST_START_DO(childEntry->root);

			while (NULL != currentNode) {
				if (J9UTF8_DATA_EQUALS(currentNode->className, currentNode->classNameLength, parentName, parentNameLength)) {
					foundRelationship = JNI_TRUE;
					break;
				}
				currentNode = J9_LINKED_LIST_NEXT_DO(childEntry->root, currentNode);
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, childNameString, childNameUTFChars);
	(*env)->ReleaseStringUTFChars(env, parentNameString, parentNameUTFChars);

	return foundRelationship;
}
