/*******************************************************************************
 * Copyright IBM Corp. and others 2024
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/
#if !defined(JFRCHUNKWRITER_HPP_)
#define JFRCHUNKWRITER_HPP_

#include "j9cfg.h"
#include "j9.h"
#include "omrlinkedlist.h"
#include "vm_api.h"

#if defined(J9VM_OPT_JFR)

#include "BufferWriter.hpp"
#include "JFRConstantPoolTypes.hpp"
#include "JFRUtils.hpp"
#include "ObjectAccessBarrierAPI.hpp"
#include "VMHelpers.hpp"

static constexpr const char * const intermediateChunkFileName = "openj9ChunkFile";

static constexpr const int JFR_HEADER_SPECIALFLAGS_COMPRESSED_INTS = 1;
static constexpr const int JFR_HEADER_SPECIALFLAGS_LAST_CHUNK = 2;

static constexpr const char * const threadStateNames[] = {
	"STATE_NEW",
	"STATE_TERMINATED",
	"STATE_RUNNABLE",
	"STATE_SLEEPING",
	"STATE_IN_OBJECT_WAIT",
	"STATE_IN_OBJECT_WAIT_TIMED",
	"STATE_PARKED",
	"STATE_PARKED_TIMED",
	"STATE_BLOCKED_ON_MONITOR_ENTER"
};

enum StringEnconding {
	NullString = 0,
	EmptyString,
	StringConstant,
	UTF8,
	UTF16,
	Latin1,
};

enum MetadataTypeID {
	ThreadStartID = 2,
	ThreadEndID = 3,
	ThreadSleepID = 4,
	JVMInformationID = 86,
	OSInformationID = 87,
	VirtualizationInformationID = 88,
	InitialSystemPropertyID = 89,
	CPUInformationID = 92,
	PhysicalMemoryID = 107,
	ExecutionSampleID = 108,
	ThreadID = 163,
	ThreadGroupID = 164,
	ClassID = 165,
	ClassLoaderID = 166,
	MethodID = 167,
	SymbolID = 168,
	ThreadStateID = 169,
	ModuleID = 185,
	PackageID = 186,
	StackTraceID = 187,
	FrameTypeID = 188,
	StackFrameID = 196,
};

enum ReservedEvent {
	EventMetadata = 0,
	EventCheckpoint,
};

enum CheckpointTypeMask {
	Generic = 0,
	Flush = 1,
	ChunkHeader = 2,
	Statics = 4,
	Thread = 8,
};

class VM_JFRChunkWriter {
	/*
	 * Data members
	 */
private:
	J9VMThread *_currentThread;
	J9JavaVM *_vm;
	BuildResult _buildResult;
	bool _debug;
	J9PortLibrary *privatePortLibrary; //PORT_ACCESS_FROM...
	bool _finalWrite;

	/* Constantpool types */
	VM_JFRConstantPoolTypes _constantPoolTypes;

	/* Processing buffers */
	StackFrame *_currentStackFrameBuffer;
	StackTraceEntry *_previousStackTraceEntry;
	StackTraceEntry *_firstStackTraceEntry;
	U_32 _currentFrameCount;
	void **_globalStringTable;
	U_8 *_jfrHeaderCursor;
	VM_BufferWriter *_bufferWriter;
	U_8 *_metadataOffset;
	U_8 *_checkPointEventOffset;
	U_8 *_previousCheckpointDelta;
	U_8 *_lastDataStart;

	static constexpr int STRING_BUFFER_LENGTH = 128;
	/* JFR CHUNK Header size */
	static constexpr int JFR_CHUNK_HEADER_SIZE = 68;

	/* conservative sizing for JFR chunk */
	static constexpr int STRING_HEADER_LENGTH = sizeof(U_64);
	static constexpr int CHECKPOINT_EVENT_HEADER_AND_FOOTER = 68;
	static constexpr int THREADSTATE_ENTRY_LENGTH = CHECKPOINT_EVENT_HEADER_AND_FOOTER + sizeof(threadStateNames) + (THREADSTATE_COUNT * STRING_HEADER_LENGTH);
	static constexpr int CLASS_ENTRY_ENTRY_SIZE = (5 * sizeof(U_64)) + sizeof(U_8);
	static constexpr int CLASSLOADER_ENTRY_SIZE = 3 * sizeof(U_64);
	static constexpr int PACKAGE_ENTRY_SIZE = (3 * sizeof(U_64)) + sizeof(U_8);
	static constexpr int METHOD_ENTRY_SIZE = (5 * sizeof(U_64)) + sizeof(U_8);
	static constexpr int THREAD_ENTRY_SIZE = 2 * sizeof(U_64);
	static constexpr int THREADGROUP_ENTRY_SIZE = 2 * sizeof(U_64);
	static constexpr int STACKTRACE_ENTRY_SIZE = (1 * sizeof(U_64)) + sizeof(U_8);
	static constexpr int MODULE_ENTRY_SIZE = 5 * sizeof(U_64);
	static constexpr int STACKFRAME_ENTRY_SIZE = 4 * sizeof(U_64);
	static constexpr int METADATA_HEADER_SIZE = 64;
	static constexpr int EXECUTION_SAMPLE_EVENT_SIZE = (5 * sizeof(U_64)) + sizeof(U_32);
	static constexpr int THREAD_START_EVENT_SIZE = (6 * sizeof(U_64)) + sizeof(U_32);
	static constexpr int THREAD_END_EVENT_SIZE = (4 * sizeof(U_64)) + sizeof(U_32);
	static constexpr int THREAD_SLEEP_EVENT_SIZE = (7 * sizeof(U_64)) + sizeof(U_32);
	static constexpr int JVM_INFORMATION_EVENT_SIZE = 3000;
	static constexpr int PHYSICAL_MEMORY_EVENT_SIZE = (4 * sizeof(U_64)) + sizeof(U_32);
	static constexpr int VIRTUALIZATION_INFORMATION_EVENT_SIZE = 50;
	static constexpr int CPU_INFORMATION_EVENT_SIZE = 600;
	static constexpr int OS_INFORMATION_EVENT_SIZE = 100;
	static constexpr int INITIAL_SYSTEM_PROPERTY_EVENT_SIZE = 6000;

	static constexpr int METADATA_ID = 1;

protected:

public:

	/*
	 * Function members
	 */
private:
	bool isResultNotOKay() {
		if (!isOkay()) {
			if (_debug) {
				j9tty_printf(PORTLIB, "Chunk writer operation failed error=%d\n", (int) _buildResult);
			}
			return true;
		}
		return false;
	}

protected:

public:
	VM_JFRChunkWriter(J9VMThread *currentThread, bool finalWrite)
		: _currentThread(currentThread)
		, _vm(currentThread->javaVM)
		, _buildResult(OK)
		, _debug(false)
		, privatePortLibrary(_vm->portLibrary)
		, _finalWrite(finalWrite)
		, _constantPoolTypes(currentThread)
		, _currentStackFrameBuffer(NULL)
		, _previousStackTraceEntry(NULL)
		, _firstStackTraceEntry(NULL)
		, _currentFrameCount(0)
		, _globalStringTable(NULL)
		, _jfrHeaderCursor(NULL)
		, _bufferWriter(NULL)
		, _metadataOffset(NULL)
		, _previousCheckpointDelta(NULL)
		, _lastDataStart(NULL)
	{
	}

	void
	loadEvents()
	{
		_constantPoolTypes.loadEvents();
		_buildResult = _constantPoolTypes.getBuildResult();
	}

	bool isOkay()
	{
		return _buildResult == OK;
	}

	BuildResult buildResult()
	{
		return _buildResult;
	}

	void
	writeIntermediateJFRChunkToFile()
	{
		UDATA written = 0;
		char fileName[sizeof(intermediateChunkFileName) + 16 + sizeof(".jfr")];
		sprintf(fileName, "%s%lX.jfr", intermediateChunkFileName, _vm->jfrState.jfrChunkCount);
		UDATA len = _bufferWriter->getSize();
		IDATA fd = j9file_open(fileName, EsOpenWrite | EsOpenCreate | EsOpenTruncate , 0666);

		if (-1 == fd) {
			_buildResult = FileIOError;
			goto done;
		}

		written = j9file_write(fd, _bufferWriter->getBufferStart(), len);

		if (len != written) {
			_buildResult = FileIOError;
		}

		if (-1 != fd) {
			j9file_close(fd);
		}
done:
		return;

	}

	void writeJFRChunk()
	{
		U_8 *buffer = NULL;
		UDATA requiredBufferSize = 0;

		if (NULL == _vm->jfrState.metaDataBlobFile) {
			_buildResult = MetaDataFileNotLoaded;
			goto done;
		}

		if (_debug) {
			_constantPoolTypes.printTables();
		}

		if (FALSE == _vm->jfrState.isConstantEventsInitialized) {
			omrthread_monitor_enter(_vm->jfrState.isConstantEventsInitializedMutex);
			if (FALSE == _vm->jfrState.isConstantEventsInitialized) {
				VM_JFRConstantPoolTypes::initializeJFRConstantEvents(_vm, _currentThread, &_buildResult);
				if (isResultNotOKay()) {
					VM_JFRConstantPoolTypes::freeJFRConstantEvents(_vm);
					goto done;
				}
				/* Ensure that initialization is complete when the initialized variable is set to true */
				VM_AtomicSupport::writeBarrier();
				_vm->jfrState.isConstantEventsInitialized = TRUE;
			}
			omrthread_monitor_exit(_vm->jfrState.isConstantEventsInitializedMutex);
		}

		requiredBufferSize = calculateRequiredBufferSize();
		if (isResultNotOKay()) {
			goto done;
		}

		buffer = (U_8 *)j9mem_allocate_memory(requiredBufferSize, J9MEM_CATEGORY_CLASSES);
		if (NULL == buffer) {
			_buildResult = OutOfMemory;
		} else {
			VM_BufferWriter writer(buffer, requiredBufferSize);

			_bufferWriter = &writer;

			/* write the header last */
			_jfrHeaderCursor = _bufferWriter->getAndIncCursor(JFR_CHUNK_HEADER_SIZE);

			/* Metadata is always written first, checkpoint events can be in any order */
			_metadataOffset = writeJFRMetadata();

			_checkPointEventOffset = writeThreadStateCheckpointEvent();

			writeFrameTypeCheckpointEvent();

			writeThreadCheckpointEvent();

			writeThreadGroupCheckpointEvent();

			writeSymbolTableCheckpointEvent();

			writeModuleCheckpointEvent();

			writeClassloaderCheckpointEvent();

			writeClassCheckpointEvent();

			writePackageCheckpointEvent();

			writeMethodCheckpointEvent();

			writeStacktraceCheckpointEvent();

			//pool_do(_constantPoolTypes.getExecutionSampleTable(), &writeExecutionSampleEvent, this);

			pool_do(_constantPoolTypes.getThreadStartTable(), &writeThreadStartEvent, this);

			pool_do(_constantPoolTypes.getThreadEndTable(), &writeThreadEndEvent, this);

			pool_do(_constantPoolTypes.getThreadSleepTable(), &writeThreadSleepEvent, this);

			/* Only write constant events in first chunk */
			if (0 == _vm->jfrState.jfrChunkCount) {
				writeJVMInformationEvent();

				writeCPUInformationEvent();

				writeVirtualizationInformationEvent();

				writeOSInformationEvent();

				writeInitialSystemPropertyEvents(_vm);
			}

			writePhysicalMemoryEvent();

			writeJFRHeader();

			if (isResultNotOKay()) {
				Trc_VM_jfr_ErrorWritingChunk(_currentThread, _buildResult);
				goto freeBuffer;
			}

			writeJFRChunkToFile();

			_vm->jfrState.jfrChunkCount += 1;

freeBuffer:
			_bufferWriter = NULL;
			j9mem_free_memory(buffer);

		}
done:
		return;
	}

	static void
	writeExecutionSampleEvent(void *anElement, void *userData)
	{
		ExecutionSampleEntry *entry = (ExecutionSampleEntry *)anElement;
		VM_JFRChunkWriter *chunkWriter = (VM_JFRChunkWriter*)userData;
		VM_BufferWriter *_bufferWriter = (VM_BufferWriter *)chunkWriter->_bufferWriter;
		J9JavaVM *vm = chunkWriter->_vm;

		/* reserve size field */
		U_8 *dataStart = _bufferWriter->getAndIncCursor(sizeof(U_32));

		/* write event type */
		_bufferWriter->writeLEB128(ExecutionSampleID);

		/* write start time */
		_bufferWriter->writeLEB128((entry->time - vm->j9ras->startTimeMillis)  * 1000000);

		/* write sampling thread index */
		_bufferWriter->writeLEB128(entry->threadIndex);

		/* stacktrace index */
		_bufferWriter->writeLEB128(entry->stackTraceIndex);

		/* thread state */
		_bufferWriter->writeLEB128(RUNNABLE);

		/* write size */
		_bufferWriter->writeLEB128PaddedU32(dataStart, _bufferWriter->getCursor() - dataStart);
	}

	static void
	writeThreadStartEvent(void *anElement, void *userData)
	{
		ThreadStartEntry *entry = (ThreadStartEntry *)anElement;
		VM_JFRChunkWriter *chunkWriter = (VM_JFRChunkWriter*)userData;
		VM_BufferWriter *_bufferWriter = (VM_BufferWriter *)chunkWriter->_bufferWriter;
		J9JavaVM *vm = chunkWriter->_vm;

		/* reserve size field */
		U_8 *dataStart = _bufferWriter->getAndIncCursor(sizeof(U_32));

		/* write event type */
		_bufferWriter->writeLEB128(ThreadStartID);

		/* write start time */
		_bufferWriter->writeLEB128((entry->time - vm->j9ras->startTimeMillis) * 1000000);

		/* write event thread index */
		_bufferWriter->writeLEB128(entry->eventThreadIndex);

		/* stacktrace index */
		_bufferWriter->writeLEB128(entry->stackTraceIndex);

		/* write thread index */
		_bufferWriter->writeLEB128(entry->threadIndex);

		/* write parent thread index */
		_bufferWriter->writeLEB128(entry->parentThreadIndex);

		/* write size */
		_bufferWriter->writeLEB128PaddedU32(dataStart, _bufferWriter->getCursor() - dataStart);
	}

	static void
	writeThreadEndEvent(void *anElement, void *userData)
	{
		ThreadEndEntry *entry = (ThreadEndEntry *)anElement;
		VM_JFRChunkWriter *chunkWriter = (VM_JFRChunkWriter*)userData;
		VM_BufferWriter *_bufferWriter = (VM_BufferWriter *)chunkWriter->_bufferWriter;
		J9JavaVM *vm = chunkWriter->_vm;

		/* reserve size field */
		U_8 *dataStart = _bufferWriter->getAndIncCursor(sizeof(U_32));

		/* write event type */
		_bufferWriter->writeLEB128(ThreadEndID);

		/* write start time */
		_bufferWriter->writeLEB128((entry->time - vm->j9ras->startTimeMillis) * 1000000);

		/* write event thread index */
		_bufferWriter->writeLEB128(entry->eventThreadIndex);

		/* write thread index */
		_bufferWriter->writeLEB128(entry->threadIndex);

		/* write size */
		_bufferWriter->writeLEB128PaddedU32(dataStart, _bufferWriter->getCursor() - dataStart);
	}

	static void
	writeThreadSleepEvent(void *anElement, void *userData)
	{
		ThreadSleepEntry *entry = (ThreadSleepEntry *)anElement;
		VM_JFRChunkWriter *chunkWriter = (VM_JFRChunkWriter*)userData;
		VM_BufferWriter *_bufferWriter = (VM_BufferWriter *)chunkWriter->_bufferWriter;
		J9JavaVM *vm = chunkWriter->_vm;

		/* reserve size field */
		U_8 *dataStart = _bufferWriter->getAndIncCursor(sizeof(U_32));

		/* write event type */
		_bufferWriter->writeLEB128(ThreadSleepID);

		/* write start time */
		_bufferWriter->writeLEB128((entry->time - vm->j9ras->startTimeMillis) * 1000000);

		/* write duration time which is always in ticks, in our case nanos */
		_bufferWriter->writeLEB128(entry->duration);

		/* write event thread index */
		_bufferWriter->writeLEB128(entry->eventThreadIndex);

		/* stacktrace index */
		_bufferWriter->writeLEB128(entry->stackTraceIndex);

		/* write sleep time which is always in millis */
		_bufferWriter->writeLEB128(entry->sleepTime/1000000);

		/* write size */
		_bufferWriter->writeLEB128PaddedU32(dataStart, _bufferWriter->getCursor() - dataStart);
	}

	void
	writeJFRChunkToFile()
	{
		UDATA len = _bufferWriter->getSize();

		UDATA written = j9file_write(_vm->jfrState.blobFileDescriptor, _bufferWriter->getBufferStart(), len);

		if (len != written) {
			_buildResult = FileIOError;
		}

		if (_debug) {
			writeIntermediateJFRChunkToFile();
		}

		return;
	}

	void writeJFRHeader();

	U_8 *writeJFRMetadata();

	U_8 *writeCheckpointEventHeader(CheckpointTypeMask typeMask, U_32 cpCount);

	void writeUTF8String(J9UTF8* string);

	void writeUTF8String(U_8 *data, UDATA len);

	void writeStringLiteral(const char *string);

	U_8 *writeThreadStateCheckpointEvent();

	U_8 *writePackageCheckpointEvent();

	U_8 *writeMethodCheckpointEvent();

	U_8 *writeClassloaderCheckpointEvent();

	U_8 *writeClassCheckpointEvent();

	U_8 *writeModuleCheckpointEvent();

	U_8 *writeThreadCheckpointEvent();

	U_8 *writeThreadGroupCheckpointEvent();

	U_8 *writeFrameTypeCheckpointEvent();

	U_8 *writeSymbolTableCheckpointEvent();

	U_8 *writeStacktraceCheckpointEvent();

	U_8 *writeJVMInformationEvent();

	U_8 *writePhysicalMemoryEvent();

	U_8 *writeCPUInformationEvent();

	U_8 *writeVirtualizationInformationEvent();

	U_8 *writeOSInformationEvent();

	void writeInitialSystemPropertyEvents(J9JavaVM *vm);

	UDATA
	calculateRequiredBufferSize()
	{
		UDATA requireBufferSize = _constantPoolTypes.getRequiredBufferSize();
		requireBufferSize += JFR_CHUNK_HEADER_SIZE;

		requireBufferSize += METADATA_HEADER_SIZE;

		requireBufferSize += _vm->jfrState.metaDataBlobFileSize;

		requireBufferSize += THREADSTATE_ENTRY_LENGTH;

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getClassCount() * CLASS_ENTRY_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getClassloaderCount() * CLASSLOADER_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getPackageCount() * PACKAGE_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getMethodCount() * METHOD_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getThreadCount() * THREAD_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getThreadGroupCount() * THREADGROUP_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getModuleCount() * MODULE_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getStackTraceCount() * STACKTRACE_ENTRY_SIZE);

		requireBufferSize += CHECKPOINT_EVENT_HEADER_AND_FOOTER + (_constantPoolTypes.getStackFrameCount() * STACKFRAME_ENTRY_SIZE);

		requireBufferSize += _constantPoolTypes.getExecutionSampleCount() * EXECUTION_SAMPLE_EVENT_SIZE;

		requireBufferSize += _constantPoolTypes.getThreadStartCount() * THREAD_START_EVENT_SIZE;

		requireBufferSize += _constantPoolTypes.getThreadEndCount() * THREAD_END_EVENT_SIZE;

		requireBufferSize += _constantPoolTypes.getThreadSleepCount() * THREAD_SLEEP_EVENT_SIZE;

		requireBufferSize += JVM_INFORMATION_EVENT_SIZE;

		requireBufferSize += OS_INFORMATION_EVENT_SIZE;

		requireBufferSize += PHYSICAL_MEMORY_EVENT_SIZE;

		requireBufferSize += VIRTUALIZATION_INFORMATION_EVENT_SIZE;

		requireBufferSize += CPU_INFORMATION_EVENT_SIZE;

		requireBufferSize += INITIAL_SYSTEM_PROPERTY_EVENT_SIZE;
		return requireBufferSize;
	}

	~VM_JFRChunkWriter()
	{
	}
};
#endif /* defined(J9VM_OPT_JFR) */
#endif /* JFRCHUNKWRITER_HPP_ */
