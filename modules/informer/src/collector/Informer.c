/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file modules/informer/src/collector/Informer.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "Config.h"

#include "informer/Informer.h"
#include "shm/PacketHeader.h"
#include "shm/ZoneHeader.h"
#include "util/StringToBool.h"

typedef struct InformerData InformerData;
struct InformerData {
	int initialized;
	uint64_t processID;
	
	pthread_t monitorThreadList[AesalonInformerMonitorThreadListSize];
	int monitorThreadListSize;
	
	pthread_t *threadList;
	int threadListSize;
	int threadCount;
	
	int shmFd;
	
	SHMHeader *shmHeader;
	
	const char *configData;
	
	uint8_t *zoneUseData;
};

static InformerData AI_InformerData;

static THREAD_SPECIFIC uint8_t *AI_Zone = NULL;
static THREAD_SPECIFIC PacketHeader *AI_ZonePacket = NULL;

static void AI_SetupSHM();
static void *AI_MapSHM(uint32_t start, uint32_t size);
static void *AI_SetupZone();
static void *AI_ReserveSpace();

void AI_SetupSHM() {
	const char *shmName = getenv("AesalonSHMName");
	if(shmName == NULL) {
		fprintf(stderr, "AesalonSHMName not set, aborting . . .\n");
		exit(1);
	}
	
	AI_InformerData.shmFd = shm_open(shmName, O_RDWR, S_IRUSR | S_IWUSR);
	
	AI_InformerData.shmHeader = AI_MapSHM(0, 1);
	AI_InformerData.configData = AI_MapSHM(1, AI_InformerData.shmHeader->configDataSize);
	AI_InformerData.zoneUseData =
		AI_MapSHM(AI_InformerData.shmHeader->configDataSize+1, AI_InformerData.shmHeader->zoneUsagePages);
}

void *AI_MapSHM(uint32_t start, uint32_t size) {
	struct stat s;
	printf("shmFd: %i\n", AI_InformerData.shmFd);
	if(fstat(AI_InformerData.shmFd, &s) != 0) {
		fprintf(stderr, "Could not fstat shared memory to determine size: %s\n", strerror(errno));
		exit(1);
	}
	
	if(s.st_size > (start+size) * AesalonPageSize) {
		if(AI_InformerData.shmHeader) sem_wait(&AI_InformerData.shmHeader->resizeSemaphore);
		
		if(ftruncate(AI_InformerData.shmFd, (start+size) * AesalonPageSize) != 0) {
			fprintf(stderr, "Could not resize shared memory.");
		}
		
		if(AI_InformerData.shmHeader) sem_post(&AI_InformerData.shmHeader->resizeSemaphore);
	}
	
	void *memory = 
		mmap(NULL, size * AesalonPageSize, PROT_READ | PROT_WRITE, 
			MAP_SHARED, AI_InformerData.shmFd, start * AesalonPageSize);
	
	return memory;
}

static void *AI_SetupZone() {
	/* While more memory is required . . . */
	while(AI_InformerData.shmHeader->zoneCount >= AI_InformerData.shmHeader->zonesAllocated) {
		/* Allocate more memory. */
		sem_wait(&AI_InformerData.shmHeader->resizeSemaphore);
		
		/* This seemingly-nonsensical statement is to account for the fact that during the wait above,
			another thread may have jumped in and allocated a zone.
		*/
		if(AI_InformerData.shmHeader->zoneCount >= AI_InformerData.shmHeader->zonesAllocated) {
			/* Allocate two zones per iteration. */
			AI_InformerData.shmHeader->shmSize += AI_InformerData.shmHeader->zoneSize * 2;
			AI_InformerData.shmHeader->zonesAllocated += 2;
			
			ftruncate(AI_InformerData.shmFd, AI_InformerData.shmHeader->shmSize * AesalonPageSize);
		}
		
		sem_post(&AI_InformerData.shmHeader->resizeSemaphore);
	}
	
	uint32_t i;
	for(i = 0; i < AI_InformerData.shmHeader->zonesAllocated; i ++) {
		if(AI_InformerData.zoneUseData[i/8] & (0x01 << (i % 8))) break;
	}
	if(i == AI_InformerData.shmHeader->zonesAllocated) {
		/* Something went pretty seriously wrong. Perhaps another target jumped in and took the spot first? */
		printf("Something very wrong occurred. Trying again . . .\n");
		/* Just temporarily use recursion to fix the problem. Note: don't try this at home . . . */
		AI_SetupZone();
	}
	AI_InformerData.zoneUseData[i/8] |= (0x01 << (i%8));
	
	AI_Zone = AI_MapSHM(
		AI_InformerData.shmHeader->zonePageOffset + (i*AI_InformerData.shmHeader->zoneSize),
		AI_InformerData.shmHeader->zoneSize);
	
	((ZoneHeader *)AI_Zone)->head = ((ZoneHeader *)AI_Zone)->tail = ZoneDataOffset;
	((ZoneHeader *)AI_Zone)->overflow = 0;
	/* TODO: implement support for finding the process ID. */
	((ZoneHeader *)AI_Zone)->processID = 0;
	((ZoneHeader *)AI_Zone)->threadID = ++AI_InformerData.shmHeader->lastThreadID;
	
	return AI_Zone;
}

static void *AI_ReserveSpace() {
	return NULL;
}

void __attribute__((constructor)) AI_Construct() {
	if(AI_InformerData.initialized == 1) return;
	pthread_t self = pthread_self();
	AI_StopCollection(self);
	AI_InformerData.initialized = 1;
	
	AI_SetupSHM();
	
	
	AI_ContinueCollection(self);
}

void __attribute__((destructor)) AI_Destruct() {

}

void AC_EXPORT AI_StartPacket(ModuleID moduleID) {
	if(AI_Zone == NULL) AI_SetupZone();
	AI_ZonePacket = AI_ReserveSpace(sizeof(PacketHeader));
}

void AC_EXPORT *AI_PacketSpace(uint32_t size) {
	return NULL;
}

void AC_EXPORT AI_EndPacket() {
	
}

const char *AI_ConfigurationString(const char *name) {
	uint32_t offset = 0;
	while(1) {
		const char *itemName = &AI_InformerData.configData[offset];
		if(itemName == 0 || itemName[0] == 0) break;
		
		int nameLength = strlen(itemName)+1;
		const char *itemData = &AI_InformerData.configData[offset+nameLength];
		if(!strcmp(name, itemName)) return itemData;
		
		int dataLength = strlen(itemData)+1;
		offset += nameLength + dataLength;
	}
	return NULL;
}

long AI_ConfigurationLong(const char *name) {
	const char *s = AI_ConfigurationString(name);
	if(s == NULL) return -1;
	long value;
	sscanf(s, "%ld", &value);
	return value;
}

int AI_ConfigurationBool(const char *name) {
	const char *s = AI_ConfigurationString(name);
	if(s == NULL) return 0;
	return StringToBool(s);
}

void AI_StopCollection(pthread_t threadID) {
	if(AI_InformerData.monitorThreadListSize < AesalonInformerMonitorThreadListSize) {
		AI_InformerData.monitorThreadList[AI_InformerData.monitorThreadListSize] = threadID;
		AI_InformerData.monitorThreadListSize ++;
	}
	else {
		fprintf(stderr,
			"Too many threads in monitor thread list, output data will be corrupted with Aesalon"
			"collection-thread data.\n");
		fprintf(stderr, "Increasing threadListSize in build/config may be a good idea.\n");
	}
}

void AI_ContinueCollection(pthread_t threadID) {
	int i = 0;
	while(i < AI_InformerData.monitorThreadListSize) {
		if(pthread_equal(AI_InformerData.monitorThreadList[i], threadID)) {
			AI_InformerData.monitorThreadListSize --;
			AI_InformerData.monitorThreadList[i] =
				AI_InformerData.monitorThreadList[AI_InformerData.monitorThreadListSize-1];
			break;
		}
		i ++;
	}
}

short AI_CollectionStatus() {
	if(AI_InformerData.initialized == 0) return 0;
	pthread_t self = pthread_self();
	int i = 0;
	while(i < AI_InformerData.monitorThreadListSize) {
		if(pthread_equal(self, AI_InformerData.monitorThreadList[i])) return 0;
		i ++;
	}
	
	return 1;
}
