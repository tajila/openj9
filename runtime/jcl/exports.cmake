################################################################################
# Copyright (c) 2019, 2019 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
################################################################################

omr_add_exports(jclse
	J9VMDllMain
	JCL_OnLoad
	JNI_OnUnload
	JVM_OnLoad
	Java_com_ibm_gpu_Kernel_launch
	Java_com_ibm_java_lang_management_internal_ClassLoadingMXBeanImpl_getLoadedClassCountImpl
	Java_com_ibm_java_lang_management_internal_ClassLoadingMXBeanImpl_getTotalLoadedClassCountImpl
	Java_com_ibm_java_lang_management_internal_ClassLoadingMXBeanImpl_getUnloadedClassCountImpl
	Java_com_ibm_java_lang_management_internal_ClassLoadingMXBeanImpl_isVerboseImpl
	Java_com_ibm_java_lang_management_internal_ClassLoadingMXBeanImpl_setVerboseImpl
	Java_com_ibm_java_lang_management_internal_CompilationMXBeanImpl_getTotalCompilationTimeImpl
	Java_com_ibm_java_lang_management_internal_CompilationMXBeanImpl_isCompilationTimeMonitoringSupportedImpl
	Java_com_ibm_java_lang_management_internal_CompilationMXBeanImpl_isJITEnabled
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getCollectionCountImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getCollectionTimeImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getLastCollectionEndTimeImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getLastCollectionStartTimeImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getMemoryUsedImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getTotalCompactsImpl
	Java_com_ibm_java_lang_management_internal_GarbageCollectorMXBeanImpl_getTotalMemoryFreedImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_createMemoryManagers
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_createMemoryPools
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getCurrentGCThreadsImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getGCMasterThreadCpuUsedImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getGCModeImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getGCSlaveThreadsCpuUsedImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getHeapMemoryUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getMaxHeapSizeImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getMaxHeapSizeLimitImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getMaximumGCThreadsImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getMinHeapSizeImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getNonHeapMemoryUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getObjectPendingFinalizationCountImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getSharedClassCacheMaxAotUnstoredBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getSharedClassCacheMaxJitDataUnstoredBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_getSharedClassCacheSoftmxUnstoredBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_isSetMaxHeapSizeSupportedImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_isVerboseImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setMaxHeapSizeImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setSharedClassCacheMaxAotBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setSharedClassCacheMaxJitDataBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setSharedClassCacheMinAotBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setSharedClassCacheMinJitDataBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setSharedClassCacheSoftmxBytesImpl
	Java_com_ibm_java_lang_management_internal_MemoryMXBeanImpl_setVerboseImpl
	Java_com_ibm_java_lang_management_internal_MemoryManagerMXBeanImpl_isManagedPoolImpl	
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getCollectionUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getCollectionUsageThresholdCountImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getCollectionUsageThresholdImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getPeakUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getPreCollectionUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getUsageThresholdCountImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_getUsageThresholdImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_isCollectionUsageThresholdExceededImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_isCollectionUsageThresholdSupportedImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_isUsageThresholdExceededImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_isUsageThresholdSupportedImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_resetPeakUsageImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_setCollectionUsageThresholdImpl
	Java_com_ibm_java_lang_management_internal_MemoryPoolMXBeanImpl_setUsageThresholdImpl
	Java_com_ibm_java_lang_management_internal_OperatingSystemMXBeanImpl_getSystemLoadAverageImpl
	Java_com_ibm_java_lang_management_internal_RuntimeMXBeanImpl_getNameImpl
	Java_com_ibm_java_lang_management_internal_RuntimeMXBeanImpl_getStartTimeImpl
	Java_com_ibm_java_lang_management_internal_RuntimeMXBeanImpl_getUptimeImpl
	Java_com_ibm_java_lang_management_internal_RuntimeMXBeanImpl_isBootClassPathSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_dumpAllThreadsImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_findDeadlockedThreadsImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_findMonitorDeadlockedThreadsImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_findNativeThreadIDImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getAllThreadIdsImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getDaemonThreadCountImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getMultiThreadInfoImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getNativeThreadIdsImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getPeakThreadCountImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getThreadCountImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getThreadCpuTimeImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getThreadInfoImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getThreadUserTimeImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_getTotalStartedThreadCountImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isCurrentThreadCpuTimeSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isObjectMonitorUsageSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isSynchronizerUsageSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isThreadContentionMonitoringEnabledImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isThreadContentionMonitoringSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isThreadCpuTimeEnabledImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_isThreadCpuTimeSupportedImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_resetPeakThreadCountImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_setThreadContentionMonitoringEnabledImpl
	Java_com_ibm_java_lang_management_internal_ThreadMXBeanImpl_setThreadCpuTimeEnabledImpl
	Java_com_ibm_jit_JITHelpers_arrayShapeElementCountMask
	Java_com_ibm_jit_JITHelpers_getNumBitsInDescriptionWord
	Java_com_ibm_jit_JITHelpers_getNumBitsInReferenceField
	Java_com_ibm_jit_JITHelpers_getNumBytesInDescriptionWord
	Java_com_ibm_jit_JITHelpers_getNumBytesInJ9ObjectHeader
	Java_com_ibm_jit_JITHelpers_getNumBytesInReferenceField
	Java_com_ibm_jit_JITHelpers_identityHashSaltPolicy
	Java_com_ibm_jit_JITHelpers_is32Bit
	Java_com_ibm_jit_JITHelpers_isPlatformLittleEndian
	Java_com_ibm_jit_JITHelpers_j9ClassBackfillOffsetOffset
	Java_com_ibm_jit_JITHelpers_j9ContiguousArrayHeaderSize
	Java_com_ibm_jit_JITHelpers_j9DiscontiguousArrayHeaderSize
	Java_com_ibm_jit_JITHelpers_j9IdentityHashDataHashData1Offset
	Java_com_ibm_jit_JITHelpers_j9IdentityHashDataHashData2Offset
	Java_com_ibm_jit_JITHelpers_j9IdentityHashDataHashData3Offset
	Java_com_ibm_jit_JITHelpers_j9IdentityHashDataHashSaltTableOffset
	Java_com_ibm_jit_JITHelpers_j9IdentityHashSaltPolicyNone
	Java_com_ibm_jit_JITHelpers_j9IdentityHashSaltPolicyRegion
	Java_com_ibm_jit_JITHelpers_j9IdentityHashSaltPolicyStandard
	Java_com_ibm_jit_JITHelpers_j9JavaVMIdentityHashDataOffset
	Java_com_ibm_jit_JITHelpers_j9ObjectContiguousLengthOffset
	Java_com_ibm_jit_JITHelpers_j9ObjectDiscontiguousLengthOffset
	Java_com_ibm_jit_JITHelpers_j9ObjectFlagsMask32
	Java_com_ibm_jit_JITHelpers_j9ObjectFlagsMask64
	Java_com_ibm_jit_JITHelpers_j9ObjectJ9ClassOffset
	Java_com_ibm_jit_JITHelpers_j9ROMArrayClassArrayShapeOffset
	Java_com_ibm_jit_JITHelpers_j9ThreadJ9JavaVMOffset
	Java_com_ibm_jit_JITHelpers_javaLangClassJ9ClassOffset
	Java_com_ibm_jit_JITHelpers_javaLangThreadJ9ThreadOffset
	Java_com_ibm_jit_JITHelpers_objectHeaderHasBeenHashedInClass
	Java_com_ibm_jit_JITHelpers_objectHeaderHasBeenMovedInClass
	Java_com_ibm_jvm_Dump_HeapDumpImpl
	Java_com_ibm_jvm_Dump_JavaDumpImpl
	Java_com_ibm_jvm_Dump_SnapDumpImpl
	Java_com_ibm_jvm_Dump_SystemDumpImpl
	Java_com_ibm_jvm_Dump_isToolDump
	Java_com_ibm_jvm_Dump_queryDumpOptionsImpl
	Java_com_ibm_jvm_Dump_resetDumpOptionsImpl
	Java_com_ibm_jvm_Dump_setDumpOptionsImpl
	Java_com_ibm_jvm_Dump_triggerDumpsImpl
	Java_com_ibm_jvm_Log_QueryOptionsImpl
	Java_com_ibm_jvm_Log_SetOptionsImpl
	Java_com_ibm_jvm_Stats_getStats
	Java_com_ibm_jvm_Trace_getMicros
	Java_com_ibm_jvm_Trace_initTraceImpl
	Java_com_ibm_jvm_Trace_registerApplicationImpl
	Java_com_ibm_jvm_Trace_resumeImpl
	Java_com_ibm_jvm_Trace_resumeThisImpl
	Java_com_ibm_jvm_Trace_setImpl
	Java_com_ibm_jvm_Trace_snapImpl
	Java_com_ibm_jvm_Trace_suspendImpl
	Java_com_ibm_jvm_Trace_suspendThisImpl
	Java_com_ibm_jvm_Trace_traceImpl__II
	Java_com_ibm_jvm_Trace_traceImpl__IIB
	Java_com_ibm_jvm_Trace_traceImpl__IIBB
	Java_com_ibm_jvm_Trace_traceImpl__IIBBB
	Java_com_ibm_jvm_Trace_traceImpl__IIBLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IIBLjava_lang_String_2B
	Java_com_ibm_jvm_Trace_traceImpl__IIC
	Java_com_ibm_jvm_Trace_traceImpl__IICC
	Java_com_ibm_jvm_Trace_traceImpl__IICCC
	Java_com_ibm_jvm_Trace_traceImpl__IICLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IICLjava_lang_String_2C
	Java_com_ibm_jvm_Trace_traceImpl__IID
	Java_com_ibm_jvm_Trace_traceImpl__IIDD
	Java_com_ibm_jvm_Trace_traceImpl__IIDDD
	Java_com_ibm_jvm_Trace_traceImpl__IIDLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IIDLjava_lang_String_2D
	Java_com_ibm_jvm_Trace_traceImpl__IIF
	Java_com_ibm_jvm_Trace_traceImpl__IIFF
	Java_com_ibm_jvm_Trace_traceImpl__IIFFF
	Java_com_ibm_jvm_Trace_traceImpl__IIFLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IIFLjava_lang_String_2F
	Java_com_ibm_jvm_Trace_traceImpl__III
	Java_com_ibm_jvm_Trace_traceImpl__IIII
	Java_com_ibm_jvm_Trace_traceImpl__IIIII
	Java_com_ibm_jvm_Trace_traceImpl__IIILjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IIILjava_lang_String_2I
	Java_com_ibm_jvm_Trace_traceImpl__IIJ
	Java_com_ibm_jvm_Trace_traceImpl__IIJJ
	Java_com_ibm_jvm_Trace_traceImpl__IIJJJ
	Java_com_ibm_jvm_Trace_traceImpl__IIJLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IIJLjava_lang_String_2J
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_Object_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_Object_2Ljava_lang_Object_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_Object_2Ljava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_Object_2Ljava_lang_String_2Ljava_lang_Object_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2B
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2BLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2C
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2CLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2D
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2DLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2F
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2FLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2I
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2ILjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2J
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2JLjava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2Ljava_lang_Object_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2Ljava_lang_Object_2Ljava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2Ljava_lang_String_2
	Java_com_ibm_jvm_Trace_traceImpl__IILjava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
	Java_com_ibm_lang_management_internal_ExtendedGarbageCollectorMXBeanImpl_getLastGcInfoImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getFreePhysicalMemorySizeImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getHardwareModelImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getMemoryUsageImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getOnlineProcessorsImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessCpuTimeImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessPhysicalMemorySizeImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessPrivateMemorySizeImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessVirtualMemorySizeImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessingCapacityImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getProcessorUsageImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getTotalPhysicalMemoryImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_getTotalProcessorUsageImpl
	Java_com_ibm_lang_management_internal_ExtendedOperatingSystemMXBeanImpl_isDLPAREnabled
	Java_com_ibm_lang_management_internal_ExtendedRuntimeMXBeanImpl_getProcessIDImpl
	Java_com_ibm_lang_management_internal_ExtendedRuntimeMXBeanImpl_getVMIdleStateImpl
	Java_com_ibm_lang_management_internal_JvmCpuMonitor_getThreadCategoryImpl
	Java_com_ibm_lang_management_internal_JvmCpuMonitor_getThreadsCpuUsageImpl
	Java_com_ibm_lang_management_internal_JvmCpuMonitor_setThreadCategoryImpl
	Java_com_ibm_lang_management_internal_MemoryNotificationThreadShutdown_sendShutdownNotification
	Java_com_ibm_lang_management_internal_MemoryNotificationThread_processNotificationLoop
	Java_com_ibm_lang_management_internal_OperatingSystemNotificationThreadShutdown_sendShutdownNotification
	Java_com_ibm_lang_management_internal_OperatingSystemNotificationThread_processNotificationLoop
	Java_com_ibm_lang_management_internal_ProcessorMXBeanImpl_getNumberCPUsImpl
	Java_com_ibm_lang_management_internal_ProcessorMXBeanImpl_setNumberActiveCPUsImpl
	Java_com_ibm_lang_management_internal_SysinfoCpuTime_getCpuUtilizationImpl
	Java_com_ibm_lang_management_internal_UnixExtendedOperatingSystem_getMaxFileDescriptorCountImpl
	Java_com_ibm_lang_management_internal_UnixExtendedOperatingSystem_getOpenFileDescriptorCountImpl
	Java_openj9_internal_tools_attach_diagnostics_base_DiagnosticUtils_getHeapClassStatisticsImpl
	Java_openj9_internal_tools_attach_diagnostics_base_DiagnosticUtils_dumpAllThreadsImpl
	Java_openj9_internal_tools_attach_diagnostics_base_DiagnosticUtils_triggerDumpsImpl
	Java_com_ibm_oti_reflect_AnnotationParser_getAnnotationsDataImpl__Ljava_lang_Class_2
	Java_com_ibm_oti_reflect_AnnotationParser_getAnnotationsData__Ljava_lang_reflect_Constructor_2
	Java_com_ibm_oti_reflect_AnnotationParser_getAnnotationsData__Ljava_lang_reflect_Field_2
	Java_com_ibm_oti_reflect_AnnotationParser_getAnnotationsData__Ljava_lang_reflect_Method_2
	Java_com_ibm_oti_reflect_AnnotationParser_getConstantPool
	Java_com_ibm_oti_reflect_AnnotationParser_getDefaultValueData
	Java_com_ibm_oti_reflect_AnnotationParser_getParameterAnnotationsData__Ljava_lang_reflect_Constructor_2
	Java_com_ibm_oti_reflect_AnnotationParser_getParameterAnnotationsData__Ljava_lang_reflect_Method_2
	Java_com_ibm_oti_reflect_TypeAnnotationParser_getTypeAnnotationsDataImpl__Ljava_lang_Class_2
	Java_com_ibm_oti_reflect_TypeAnnotationParser_getTypeAnnotationsDataImpl__Ljava_lang_reflect_Constructor_2
	Java_com_ibm_oti_reflect_TypeAnnotationParser_getTypeAnnotationsDataImpl__Ljava_lang_reflect_Field_2
	Java_com_ibm_oti_reflect_TypeAnnotationParser_getTypeAnnotationsDataImpl__Ljava_lang_reflect_Method_2
	Java_com_ibm_oti_shared_SharedAbstractHelper_getIsVerboseImpl
	Java_com_ibm_oti_shared_SharedClassAbstractHelper_initializeShareableClassloaderImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_freeSpaceBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_maxAotBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_maxJitDataBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_maxSizeBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_minAotBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_minJitDataBytesImpl
	Java_com_ibm_oti_shared_SharedClassStatistics_softmxBytesImpl
	Java_com_ibm_oti_shared_SharedClassTokenHelperImpl_findSharedClassImpl2
	Java_com_ibm_oti_shared_SharedClassTokenHelperImpl_storeSharedClassImpl2
	Java_com_ibm_oti_shared_SharedClassURLClasspathHelperImpl_findSharedClassImpl2
	Java_com_ibm_oti_shared_SharedClassURLClasspathHelperImpl_init
	Java_com_ibm_oti_shared_SharedClassURLClasspathHelperImpl_notifyClasspathChange2
	Java_com_ibm_oti_shared_SharedClassURLClasspathHelperImpl_notifyClasspathChange3
	Java_com_ibm_oti_shared_SharedClassURLClasspathHelperImpl_storeSharedClassImpl2
	Java_com_ibm_oti_shared_SharedClassURLHelperImpl_findSharedClassImpl3
	Java_com_ibm_oti_shared_SharedClassURLHelperImpl_init
	Java_com_ibm_oti_shared_SharedClassURLHelperImpl_storeSharedClassImpl3
	Java_com_ibm_oti_shared_SharedClassUtilities_destroySharedCacheImpl
	Java_com_ibm_oti_shared_SharedClassUtilities_getSharedCacheInfoImpl
	Java_com_ibm_oti_shared_SharedClassUtilities_init
	Java_com_ibm_oti_shared_SharedDataHelperImpl_findSharedDataImpl
	Java_com_ibm_oti_shared_SharedDataHelperImpl_storeSharedDataImpl
	Java_com_ibm_oti_shared_Shared_isNonBootSharingEnabledImpl
	Java_com_ibm_oti_vm_BootstrapClassLoader_addJar
	Java_com_ibm_oti_vm_ORBVMHelpers_LatestUserDefinedLoader
	Java_com_ibm_oti_vm_ORBVMHelpers_getNumBitsInDescriptionWord
	Java_com_ibm_oti_vm_ORBVMHelpers_getNumBitsInReferenceField
	Java_com_ibm_oti_vm_ORBVMHelpers_getNumBytesInDescriptionWord
	Java_com_ibm_oti_vm_ORBVMHelpers_getNumBytesInJ9ObjectHeader
	Java_com_ibm_oti_vm_ORBVMHelpers_getNumBytesInReferenceField
	Java_com_ibm_oti_vm_ORBVMHelpers_is32Bit
	Java_com_ibm_oti_vm_VM_allInstances
	Java_com_ibm_oti_vm_VM_dumpString
	Java_com_ibm_oti_vm_VM_getClassNameImpl
	Java_com_ibm_oti_vm_VM_getClassPathCount
	Java_com_ibm_oti_vm_VM_getNonBootstrapClassLoader
	Java_com_ibm_oti_vm_VM_getPathFromClassPath
	Java_com_ibm_oti_vm_VM_getVMArgsImpl
	Java_com_ibm_oti_vm_VM_globalGC
	Java_com_ibm_oti_vm_VM_localGC
	Java_com_ibm_oti_vm_VM_markCurrentThreadAsSystemImpl
	Java_com_ibm_oti_vm_VM_setCommonData
	Java_com_ibm_rmi_io_IIOPInputStream_00024LUDCLStackWalkOptimizer_LUDCLMarkFrame
	Java_com_ibm_rmi_io_IIOPInputStream_00024LUDCLStackWalkOptimizer_LUDCLUnmarkFrameImpl
	Java_com_ibm_virtualization_management_internal_GuestOS_retrieveMemoryUsageImpl
	Java_com_ibm_virtualization_management_internal_GuestOS_retrieveProcessorUsageImpl
	Java_com_ibm_virtualization_management_internal_HypervisorMXBeanImpl_getVendorImpl
	Java_com_ibm_virtualization_management_internal_HypervisorMXBeanImpl_isEnvironmentVirtualImpl
	Java_java_lang_Access_getConstantPool
	Java_java_lang_ClassLoader_defineClassImpl
	Java_java_lang_ClassLoader_foundJavaAssertOption
	Java_java_lang_ClassLoader_isVerboseImpl
	Java_java_lang_Class_allocateAndFillArray
	Java_java_lang_Class_getConstructorImpl
	Java_java_lang_Class_getConstructorsImpl
	Java_java_lang_Class_getDeclaredAnnotationsData
	Java_java_lang_Class_getDeclaredClassesImpl
	Java_java_lang_Class_getDeclaredConstructorImpl
	Java_java_lang_Class_getDeclaredConstructorsImpl
	Java_java_lang_Class_getDeclaredFieldImpl
	Java_java_lang_Class_getDeclaredFieldsImpl
	Java_java_lang_Class_getDeclaredMethodImpl
	Java_java_lang_Class_getDeclaredMethodsImpl
	Java_java_lang_Class_getDeclaringClassImpl
	Java_java_lang_Class_getEnclosingObject
	Java_java_lang_Class_getEnclosingObjectClass
	Java_java_lang_Class_getFieldImpl
	Java_java_lang_Class_getFieldsImpl
	Java_java_lang_Class_getGenericSignature
	Java_java_lang_Class_getMethodImpl
	Java_java_lang_Class_getStackClasses
	Java_java_lang_Class_getStaticMethodCountImpl
	Java_java_lang_Class_getStaticMethodsImpl
	Java_java_lang_Class_getVirtualMethodCountImpl
	Java_java_lang_Class_getVirtualMethodsImpl
	Java_java_lang_Class_isClassADeclaredClass
	Java_java_lang_Compiler_commandImpl
	Java_java_lang_Compiler_compileClassImpl
	Java_java_lang_Compiler_compileClassesImpl
	Java_java_lang_Compiler_disable
	Java_java_lang_Compiler_enable
	Java_java_lang_J9VMInternals_dumpString
	Java_java_lang_J9VMInternals_getStackTrace
	Java_java_lang_J9VMInternals_newInstance
	Java_java_lang_System_getEncoding
	Java_java_lang_System_getPropertyList
	Java_java_lang_System_mapLibraryName
	Java_java_lang_System_rasInitializeVersion
	Java_java_lang_System_setFieldImpl
	Java_java_lang_System_startSNMPAgent
	Java_java_lang_Thread_getStackTraceImpl
	Java_java_lang_Thread_getStateImpl
	Java_java_lang_Thread_holdsLock
	Java_java_lang_Thread_interruptImpl
	Java_java_lang_Thread_resumeImpl
	Java_java_lang_Thread_setNameImpl
	Java_java_lang_Thread_setPriorityNoVMAccessImpl
	Java_java_lang_Thread_startImpl
	Java_java_lang_Thread_stopImpl
	Java_java_lang_Thread_suspendImpl
	Java_java_lang_Thread_yield
	Java_java_lang_invoke_InterfaceHandle_registerNatives
	Java_java_lang_invoke_MethodHandle_getCPClassNameAt
	Java_java_lang_invoke_MethodHandle_getCPMethodHandleAt
	Java_java_lang_invoke_MethodHandle_getCPMethodTypeAt
	Java_java_lang_invoke_MethodHandle_getCPTypeAt
	Java_java_lang_invoke_MethodHandle_invoke
	Java_java_lang_invoke_MethodHandle_invokeExact
	Java_java_lang_invoke_MethodHandle_requestCustomThunkFromJit
	Java_java_lang_invoke_MethodHandle_vmRefFieldOffset
	Java_java_lang_invoke_MethodType_makeTenured
	Java_java_lang_invoke_MutableCallSite_freeGlobalRef
	Java_java_lang_invoke_MutableCallSite_registerNatives
	Java_java_lang_invoke_PrimitiveHandle_lookupField
	Java_java_lang_invoke_PrimitiveHandle_lookupMethod
	Java_java_lang_invoke_PrimitiveHandle_setVMSlotAndRawModifiersFromConstructor
	Java_java_lang_invoke_PrimitiveHandle_setVMSlotAndRawModifiersFromField
	Java_java_lang_invoke_PrimitiveHandle_setVMSlotAndRawModifiersFromMethod
	Java_java_lang_invoke_PrimitiveHandle_setVMSlotAndRawModifiersFromSpecialHandle
	Java_java_lang_invoke_ThunkTuple_registerNatives
	Java_java_lang_ref_Finalizer_runAllFinalizersImpl
	Java_java_lang_ref_Finalizer_runFinalizationImpl
	Java_java_lang_ref_Reference_reprocess
	Java_java_lang_ref_Reference_waitForReferenceProcessingImpl
	Java_java_lang_reflect_Array_multiNewArrayImpl
	Java_java_lang_reflect_Proxy_defineClass0__Ljava_lang_ClassLoader_2Ljava_lang_String_2_3BII
	Java_java_lang_reflect_Proxy_defineClass0__Ljava_lang_ClassLoader_2Ljava_lang_String_2_3BIILjava_lang_Object_2_3Ljava_lang_Object_2Ljava_lang_Object_2
	Java_java_lang_reflect_Proxy_defineClassImpl
	Java_java_security_AccessController_getAccSnapshot
	Java_java_security_AccessController_getCallerPD
	Java_java_security_AccessController_initializeInternal
	Java_java_util_stream_IntPipeline_promoteGPUCompile
	Java_jdk_internal_misc_Unsafe_allocateDBBMemory
	Java_jdk_internal_misc_Unsafe_copySwapMemory0
	Java_jdk_internal_misc_Unsafe_freeDBBMemory
	Java_jdk_internal_misc_Unsafe_reallocateDBBMemory
	Java_jdk_internal_misc_Unsafe_registerNatives
	Java_jdk_internal_misc_Unsafe_shouldBeInitialized
	Java_jdk_internal_perf_Perf_registerNatives
	Java_sun_misc_Perf_attach
	Java_sun_misc_Perf_createByteArray
	Java_sun_misc_Perf_createLong
	Java_sun_misc_Perf_detach
	Java_sun_misc_Perf_highResCounter
	Java_sun_misc_Perf_highResFrequency
	Java_sun_misc_Perf_registerNatives
	Java_sun_misc_Unsafe_allocateDBBMemory
	Java_sun_misc_Unsafe_allocateMemory
	Java_sun_misc_Unsafe_allocateMemoryNoException
	Java_sun_misc_Unsafe_copyMemory__Ljava_lang_Object_2JLjava_lang_Object_2JJ
	Java_sun_misc_Unsafe_defineAnonymousClass
	Java_sun_misc_Unsafe_defineClass__Ljava_lang_String_2_3BIILjava_lang_ClassLoader_2Ljava_security_ProtectionDomain_2
	Java_sun_misc_Unsafe_ensureClassInitialized
	Java_sun_misc_Unsafe_freeDBBMemory
	Java_sun_misc_Unsafe_freeMemory
	Java_sun_misc_Unsafe_getJavaMirror
	Java_sun_misc_Unsafe_getKlassPointer
	Java_sun_misc_Unsafe_getLoadAverage
	Java_sun_misc_Unsafe_getUncompressedObject
	Java_sun_misc_Unsafe_isBigEndian0
	Java_sun_misc_Unsafe_monitorEnter
	Java_sun_misc_Unsafe_monitorExit
	Java_sun_misc_Unsafe_objectFieldOffset
	Java_sun_misc_Unsafe_pageSize
	Java_sun_misc_Unsafe_park
	Java_sun_misc_Unsafe_reallocateDBBMemory
	Java_sun_misc_Unsafe_reallocateMemory
	Java_sun_misc_Unsafe_registerNatives
	Java_sun_misc_Unsafe_setMemory__Ljava_lang_Object_2JJB
	Java_sun_misc_Unsafe_staticFieldBase__Ljava_lang_reflect_Field_2
	Java_sun_misc_Unsafe_staticFieldOffset
	Java_sun_misc_Unsafe_throwException
	Java_sun_misc_Unsafe_tryMonitorEnter
	Java_sun_misc_Unsafe_unalignedAccess0
	Java_sun_misc_Unsafe_unpark
	Java_sun_reflect_ConstantPool_getClassAt0
	Java_sun_reflect_ConstantPool_getClassAtIfLoaded0
	Java_sun_reflect_ConstantPool_getDoubleAt0
	Java_sun_reflect_ConstantPool_getFieldAt0
	Java_sun_reflect_ConstantPool_getFieldAtIfLoaded0
	Java_sun_reflect_ConstantPool_getFloatAt0
	Java_sun_reflect_ConstantPool_getIntAt0
	Java_sun_reflect_ConstantPool_getLongAt0
	Java_sun_reflect_ConstantPool_getMemberRefInfoAt0
	Java_sun_reflect_ConstantPool_getMethodAt0
	Java_sun_reflect_ConstantPool_getMethodAtIfLoaded0
	Java_sun_reflect_ConstantPool_getSize0
	Java_sun_reflect_ConstantPool_getStringAt0
	Java_sun_reflect_ConstantPool_getUTF8At0
)

if(J9VM_OPT_SIDECAR)
	omr_add_exports(jclse
		Java_openj9_internal_tools_attach_target_CommonDirectory_getFileOwner
		Java_openj9_internal_tools_attach_target_FileLock_lockFileImpl
		Java_openj9_internal_tools_attach_target_FileLock_unlockFileImpl
		Java_openj9_internal_tools_attach_target_IPC_cancelNotify
		Java_openj9_internal_tools_attach_target_IPC_chmod
		Java_openj9_internal_tools_attach_target_IPC_chownFileToTargetUid
		Java_openj9_internal_tools_attach_target_IPC_closeSemaphore
		Java_openj9_internal_tools_attach_target_IPC_createFileWithPermissionsImpl
		Java_openj9_internal_tools_attach_target_IPC_destroySemaphore
		Java_openj9_internal_tools_attach_target_IPC_getProcessId
		Java_openj9_internal_tools_attach_target_IPC_getTempDirImpl
		Java_openj9_internal_tools_attach_target_IPC_getUid
		Java_openj9_internal_tools_attach_target_IPC_isUsingDefaultUid
		Java_openj9_internal_tools_attach_target_IPC_mkdirWithPermissionsImpl
		Java_openj9_internal_tools_attach_target_IPC_notifyVm
		Java_openj9_internal_tools_attach_target_IPC_openSemaphore
		Java_openj9_internal_tools_attach_target_IPC_processExistsImpl
		Java_openj9_internal_tools_attach_target_IPC_setupSemaphore
		Java_openj9_internal_tools_attach_target_IPC_tracepoint
		Java_openj9_internal_tools_attach_target_IPC_waitSemaphore
	)
endif()

if(OMR_ENV_DATA32)
	omr_add_exports(jclse
		Java_com_ibm_jit_JITHelpers_getArrayShapeFromRomClass32
		Java_com_ibm_jit_JITHelpers_getBackfillOffsetFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getClassDepthAndFlagsFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getClassFlagsFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getClassFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getDescriptionWordFromPtr32
		Java_com_ibm_jit_JITHelpers_getInstanceDescriptionFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getJ9ClassFromClass32
		Java_com_ibm_jit_JITHelpers_getModifiersFromRomClass32
		Java_com_ibm_jit_JITHelpers_getRomClassFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getSuperClassesFromJ9Class32
		Java_com_ibm_jit_JITHelpers_getTotalInstanceSizeFromJ9Class32
		Java_com_ibm_oti_vm_ORBVMHelpers_getDescriptionWordFromPtr32
		Java_com_ibm_oti_vm_ORBVMHelpers_getInstanceDescriptionFromJ9Class32
		Java_com_ibm_oti_vm_ORBVMHelpers_getJ9ClassFromClass32
		Java_com_ibm_oti_vm_ORBVMHelpers_getTotalInstanceSizeFromJ9Class32
	)
else()
	omr_add_exports(jclse
		Java_com_ibm_jit_JITHelpers_getArrayShapeFromRomClass64
		Java_com_ibm_jit_JITHelpers_getBackfillOffsetFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getClassDepthAndFlagsFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getClassFlagsFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getClassFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getDescriptionWordFromPtr64
		Java_com_ibm_jit_JITHelpers_getInstanceDescriptionFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getJ9ClassFromClass64
		Java_com_ibm_jit_JITHelpers_getModifiersFromRomClass64
		Java_com_ibm_jit_JITHelpers_getRomClassFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getSuperClassesFromJ9Class64
		Java_com_ibm_jit_JITHelpers_getTotalInstanceSizeFromJ9Class64
		Java_com_ibm_oti_vm_ORBVMHelpers_getDescriptionWordFromPtr64
		Java_com_ibm_oti_vm_ORBVMHelpers_getInstanceDescriptionFromJ9Class64
		Java_com_ibm_oti_vm_ORBVMHelpers_getJ9ClassFromClass64
		Java_com_ibm_oti_vm_ORBVMHelpers_getTotalInstanceSizeFromJ9Class64
	)
endif()


if(JAVA_SPEC_VERSION EQUAL 8)
	omr_add_exports(jclse Java_sun_misc_URLClassPath_getLookupCacheURLs)
endif()

# java 9+
if(NOT JAVA_SPEC_VERSION LESS 10)
	omr_add_exports(jclse
		Java_java_lang_StackWalker_getImpl
		Java_java_lang_StackWalker_walkWrapperImpl
		Java_java_lang_invoke_FieldVarHandle_lookupField
		Java_java_lang_invoke_FieldVarHandle_unreflectField
		Java_java_lang_invoke_VarHandle_addAndGet
		Java_java_lang_invoke_VarHandle_compareAndExchange
		Java_java_lang_invoke_VarHandle_compareAndExchangeAcquire
		Java_java_lang_invoke_VarHandle_compareAndExchangeRelease
		Java_java_lang_invoke_VarHandle_compareAndSet
		Java_java_lang_invoke_VarHandle_get
		Java_java_lang_invoke_VarHandle_getAcquire
		Java_java_lang_invoke_VarHandle_getAndAdd
		Java_java_lang_invoke_VarHandle_getAndAddAcquire
		Java_java_lang_invoke_VarHandle_getAndAddRelease
		Java_java_lang_invoke_VarHandle_getAndBitwiseAnd
		Java_java_lang_invoke_VarHandle_getAndBitwiseAndAcquire
		Java_java_lang_invoke_VarHandle_getAndBitwiseAndRelease
		Java_java_lang_invoke_VarHandle_getAndBitwiseOr
		Java_java_lang_invoke_VarHandle_getAndBitwiseOrAcquire
		Java_java_lang_invoke_VarHandle_getAndBitwiseOrRelease
		Java_java_lang_invoke_VarHandle_getAndBitwiseXor
		Java_java_lang_invoke_VarHandle_getAndBitwiseXorAcquire
		Java_java_lang_invoke_VarHandle_getAndBitwiseXorRelease
		Java_java_lang_invoke_VarHandle_getAndSet
		Java_java_lang_invoke_VarHandle_getAndSetAcquire
		Java_java_lang_invoke_VarHandle_getAndSetRelease
		Java_java_lang_invoke_VarHandle_getOpaque
		Java_java_lang_invoke_VarHandle_getVolatile
		Java_java_lang_invoke_VarHandle_set
		Java_java_lang_invoke_VarHandle_setOpaque
		Java_java_lang_invoke_VarHandle_setRelease
		Java_java_lang_invoke_VarHandle_setVolatile
		Java_java_lang_invoke_VarHandle_weakCompareAndSet
		Java_java_lang_invoke_VarHandle_weakCompareAndSetAcquire
		Java_java_lang_invoke_VarHandle_weakCompareAndSetPlain
		Java_java_lang_invoke_VarHandle_weakCompareAndSetRelease
		Java_jdk_internal_reflect_ConstantPool_getClassRefIndexAt0
		Java_jdk_internal_reflect_ConstantPool_getNameAndTypeRefIndexAt0
		Java_jdk_internal_reflect_ConstantPool_getNameAndTypeRefInfoAt0
		Java_jdk_internal_reflect_ConstantPool_getTagAt0
	)
	if(J9VM_OPT_PANAMA)
		omr_add_exports(jclse Java_java_lang_invoke_MethodHandles_findNativeAddress)
	endif()
endif()

#java 11+
if(NOT JAVA_SPEC_VERSION LESS 12)
	omr_add_exports(jclse
		Java_java_lang_Class_getNestHostImpl
		Java_java_lang_Class_getNestMembersImpl
		Java_java_lang_invoke_MethodHandle_getCPConstantDynamicAt
	)
endif()
