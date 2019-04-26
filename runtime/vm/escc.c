#include "j9.h"
#include "j9nonbuilder.h"
#include "j9port.h"
#include "vm_api.h"

static const U_64 esccHeader[] = {0xFFFFCAC4E001FFFF};

static const U_64 classLoaderHeader[] = {0xFFFFC1A551D5FFFF};
static const U_64 classLoaderFooter[] = {0x0000C1A551D50000};

static BOOLEAN
saveClassLoader(J9JavaVM *vm, J9ClassLoader *classLoader, IDATA fileHandle);

static BOOLEAN
restoreClassLoader(J9JavaVM *vm, J9ClassLoader **classLoader, IDATA fileHandle);

BOOLEAN
saveRAMState(J9JavaVM *vm)
{
	BOOLEAN rc = TRUE;
	IDATA fileHandle = 0;
	PORT_ACCESS_FROM_JAVAVM(vm);

	if (NULL == vm->esccName) {
		rc = FALSE;
		goto done;
	}

	fileHandle = j9file_open(vm->esccName, EsOpenCreate | EsOpenCreateAlways | EsOpenRead | EsOpenWrite, 0777);
	if (0 == fileHandle) {
		rc = FALSE;
		goto done;
	}

	/* write header */
	if (-1 == j9file_write(fileHandle, esccHeader, sizeof(esccHeader))) {
		rc = FALSE;
		goto done;
	}

	if (!saveClassLoader(vm, vm->systemClassLoader, fileHandle)) {
		rc = FALSE;
		goto done;
	}

done:
	return rc;
}

static BOOLEAN
saveClassLoader(J9JavaVM *vm, J9ClassLoader *classLoader, IDATA fileHandle) {
	PORT_ACCESS_FROM_JAVAVM(vm);
	BOOLEAN rc = TRUE;

	/* write header */
	if (-1 == j9file_write(fileHandle, classLoaderHeader, sizeof(classLoaderHeader))) {
		rc = FALSE;
		goto done;
	}

	classLoader->sharedLibraries = NULL;
	classLoader->classHashTable  = NULL;
	classLoader->romClassOrphansHashTable  = NULL;
	classLoader->classLoaderObject = NULL;
	classLoader->classPathEntries = NULL;
	classLoader->classPathEntryCount = 0;
	classLoader->unloadLink = NULL;
	classLoader->gcLinkNext = NULL;
	classLoader->gcLinkPrevious = NULL;
	classLoader->gcFlags = 0;
	classLoader->gcThreadNotification = NULL;
	classLoader->jniIDs = NULL;
	classLoader->flags = 0;
#if defined(J9VM_NEEDS_JNI_REDIRECTION)
	classLoader->jniRedirectionBlocks = NULL;
#endif /* J9VM_NEEDS_JNI_REDIRECTION */
	classLoader->jitMetaDataList = NULL;
	classLoader->classSegments = NULL;
	classLoader->ramClassLargeBlockFreeList = NULL;
	classLoader->ramClassSmallBlockFreeList = NULL;
	classLoader->ramClassTinyBlockFreeList = NULL;
	classLoader->ramClassUDATABlockFreeList = NULL;
	classLoader->redefinedClasses = NULL;
	classLoader->librariesHead = NULL;
	classLoader->librariesTail = NULL;
	classLoader->gcRememberedSet = 0;
	classLoader->moduleHashTable = NULL;
	classLoader->packageHashTable = NULL;
	classLoader->moduleExtraInfoHashTable = NULL;
	classLoader->classLocationHashTable = NULL;

	/* write classLoader struct */
	if (-1 == j9file_write(fileHandle, classLoader, sizeof(J9ClassLoader))) {
		rc = FALSE;
		goto done;
	}

	/* write footer */
	if (-1 == j9file_write(fileHandle, classLoaderFooter, sizeof(classLoaderFooter))) {
		rc = FALSE;
		goto done;
	}

done:
	return rc;
}

BOOLEAN
restoreRAMState(J9JavaVM *vm)
{
	PORT_ACCESS_FROM_JAVAVM(vm);
	BOOLEAN rc = TRUE;
	IDATA fileHandle = 0;

	fileHandle = j9file_open(vm->esccName, EsOpenRead, 0);
	if (0 == fileHandle) {
		rc = FALSE;
		goto done;
	}

	if (!restoreClassLoader(vm, &vm->systemClassLoader, fileHandle)) {
		rc = FALSE;
		goto done;
	}

done:
	return rc;
}

static BOOLEAN
restoreClassLoader(J9JavaVM *vm, J9ClassLoader **classLoader, IDATA fileHandle)
{
	PORT_ACCESS_FROM_JAVAVM(vm);
	BOOLEAN rc = TRUE;

	ACQUIRE_CLASS_LOADER_BLOCKS_MUTEX(vm);

	*classLoader = pool_newElement(vm->classLoaderBlocks);

	if (NULL == *classLoader) {
		rc = FALSE;
		goto done;
	}

	if (-1 == j9file_seek(fileHandle, sizeof(esccHeader) + sizeof(classLoaderHeader), 0)) {
		rc = FALSE;
		goto done;
	}

	if (-1 == j9file_read(fileHandle, *classLoader, sizeof(J9ClassLoader))) {
		rc = FALSE;
		goto done;
	}

	(*classLoader)->classHashTable = hashClassTableNew(vm, INITIAL_CLASSHASHTABLE_SIZE);
	(*classLoader)->moduleHashTable = hashModuleTableNew(vm, INITIAL_MODULE_HASHTABLE_SIZE);
	(*classLoader)->packageHashTable = hashPackageTableNew(vm, INITIAL_PACKAGE_HASHTABLE_SIZE);
	/* Allocate classLocationHashTable only for bootloader which is the first classloader to be allocated.
	 * The classLoader being allocated must be the bootloader if javaVM->systemClassLoader is NULL.
	 */
	if ((*classLoader) == vm->systemClassLoader) {
		(*classLoader)->classLocationHashTable = hashClassLocationTableNew(vm, INITIAL_CLASSLOCATION_HASHTABLE_SIZE);
	}

	TRIGGER_J9HOOK_VM_CLASS_LOADER_CREATED(vm->hookInterface, vm, *classLoader);

	RELEASE_CLASS_LOADER_BLOCKS_MUTEX(vm);

done:
	return rc;
}
