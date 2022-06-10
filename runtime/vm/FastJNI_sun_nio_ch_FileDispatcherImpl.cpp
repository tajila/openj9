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

#include "fastJNI.h"

#include "j9protos.h"
#include "j9consts.h"
#include "j9.h"
#include "j9cfg.h"
#include "j9protos.h"

#include <sys/types.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <errno.h>

extern "C" {

//@Native public static final int EOF = -1;              // End of file
//@Native public static final int UNAVAILABLE = -2;      // Nothing available (non-blocking)
//@Native public static final int INTERRUPTED = -3;      // System call interrupted
//@Native public static final int UNSUPPORTED = -4;      // Operation not supported
//@Native public static final int THROWN = -5;           // Exception thrown in JNI code
//@Native public static final int UNSUPPORTED_CASE = -6; // This case not supported


#define IOS_EOF              (-1)
#define IOS_UNAVAILABLE      (-2)
#define IOS_INTERRUPTED      (-3)
#define IOS_UNSUPPORTED      (-4)
#define IOS_THROWN           (-5)
#define IOS_UNSUPPORTED_CASE (-6)

jlong
convertLongReturnVal(J9VMThread *currentThread, I_64 n, jboolean reading)
{
    if (n > 0) /* Number of bytes written */
        return n;
    else if (n == 0) {
        if (reading) {
            return IOS_EOF; /* EOF is -1 in javaland */
        } else {
            return 0;
        }
    }
    else if (errno == EAGAIN || errno == EWOULDBLOCK)
        return IOS_UNAVAILABLE;
    else if (errno == EINTR)
        return IOS_INTERRUPTED;
    else {
        const char *msg = reading ? "Read failed" : "Write failed";
        setCurrentExceptionUTF(currentThread, J9VMCONSTANTPOOL_JAVAIOIOEXCEPTION, msg);
        return IOS_THROWN;
    }
}

jint
fdval(J9VMThread *currentThread, j9object_t fdo)
{
	return J9VMJAVAIOFILEDESCRIPTOR_FD(currentThread, fdo);
}

jlong JNICALL
Fast_sun_nio_ch_FileDispatcherImpl_writev0(J9VMThread *currentThread, j9object_t fdo, I_64 address, I_32 len)
{
    I_32 fd = fdval(currentThread, fdo);
    struct iovec *iov = (struct iovec *)(UDATA)address;
    return convertLongReturnVal(currentThread, writev(fd, iov, len), JNI_FALSE);
}

J9_FAST_JNI_METHOD_TABLE(sun_nio_ch_FileDispatcherImpl)
	J9_FAST_JNI_METHOD("writev0", "(Ljava/io/FileDescriptor;JI)J", Fast_sun_nio_ch_FileDispatcherImpl_writev0,
		J9_FAST_JNI_RETAIN_VM_ACCESS | J9_FAST_JNI_DO_NOT_WRAP_OBJECTS | J9_FAST_JNI_DO_NOT_PASS_RECEIVER)
J9_FAST_JNI_METHOD_TABLE_END

}
