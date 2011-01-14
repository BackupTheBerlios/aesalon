/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file modules/informer/src/collector/informer.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "informer/Informer.h"
#include "common/Config.h"
#include "common/ZoneHeader.h"

typedef struct Zone_t Zone_t;
struct Zone_t {
	int offset;
	Zone_t *next;
};

typedef struct InformerData_t InformerData_t;
struct InformerData_t {
	int initialized;
	uint64_t processID;
	
	pthread_t monitorThreadList[AesalonInformerMonitorThreadListSize];
	int monitorThreadListSize;
	
	pthread_t *threadList;
	int threadListSize;
	int threadCount;
	
	int shmFd;
	
	SharedMemoryHeader_t *shmHeader;
	
	const char *configData;
	
	uint8_t *zoneUseData;
};

static InformerData_t AI_InformerData;

static __thread uint8_t *AI_Zone = NULL;
static __thread SHMPacketHeader_t *AI_ZonePacket = NULL;

/** Interally-used function; opens shared memory for later use.
	@param name The name of the SHM to use.
*/
static void AI_OpenSHM(const char *name);

static void AI_SetupHeader();
static void AI_SetupConfig();
static void AI_SetupZoneUse();

static void AI_SetupZone();
static void *AI_ReserveSpace(uint32_t amount);

static int AI_ZoneAvailable(uint32_t id);
static void AI_MarkZone(uint32_t id);
static void AI_ClearZone(uint32_t id);

/** Internally-used function to calculate the amount of space remaining in the zone for
	the current thread.
	@param zoneID The zone to calculate the remaining space.
*/
static uint32_t AI_RemainingSpace();

/* ------------------------------------------------------------------ */

void __attribute__((constructor)) AI_Construct() {
	/* By default, .initialized will be set to 0 (AI_InformerData is a global). */
	if(AI_InformerData.initialized) return;
	AI_InformerData.initialized = 1;
	
	printf("[AI] **** Constructing Informer . . .\n");
	
	pthread_t self = pthread_self();
	
	AI_StopCollection(self);
	
	pid_t pid = getpid();
	
	char filename[1024];
	
	int fd = open("/proc/self/cmdline", O_RDONLY);
	
	read(fd, filename, sizeof(filename));
	
	close(fd);
	
	/* String hashing algorithm: djb2. */
	
	uint64_t pathHash = 0;
	int c = 0;
	char *p = filename;
	while((c = (*p++))) {
		pathHash = c + (pathHash << 6) + (pathHash << 16) - pathHash;
	}
	
	/* Clear the first 16 bits for the PID to be inserted properly. */
	pathHash &= ~0xffff;
	
	AI_InformerData.processID = pathHash ^ pid;
	
	const char *shmName = getenv("AesalonSHMName");
	if(shmName == NULL) {
		fprintf(stderr, "[aesalon] AesalonSHMName not set, aborting.\n");
		exit(1);
	}
	AI_OpenSHM(shmName);
	
	AI_SetupHeader();
	AI_SetupConfig();
	AI_SetupZoneUse();
	
	AI_InformerData.threadList = malloc(sizeof(pthread_t) * 16);
	AI_InformerData.threadListSize = 16;
	AI_InformerData.threadCount = 1;
	AI_InformerData.threadList[0] = self;
	
	AI_ContinueCollection(self);
	
	AI_StartPacket(0);
	AI_PacketSpace(32);
	AI_EndPacket();
}

void __attribute__((destructor)) AI_Destruct() {
	printf("[AI] Destructing Informer . . .\n");
}

static void AI_OpenSHM(const char *name) {
	AI_InformerData.shmFd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
}

static void AI_SetupHeader() {
	AI_InformerData.shmHeader = mmap(NULL, AesalonPageSize,
		PROT_READ | PROT_WRITE, MAP_SHARED, AI_InformerData.shmFd, 0);
}

static void AI_SetupConfig() {
	AI_InformerData.configData = mmap(NULL, AI_InformerData.shmHeader->configDataSize*AesalonPageSize,
		PROT_READ | PROT_WRITE, MAP_SHARED, AI_InformerData.shmFd, AesalonPageSize);
}

static void AI_SetupZoneUse() {
	AI_InformerData.zoneUseData = mmap(NULL, AI_InformerData.shmHeader->zoneUsagePages*AesalonPageSize,
		PROT_READ | PROT_WRITE, MAP_SHARED, AI_InformerData.shmFd,
		(AI_InformerData.shmHeader->configDataSize + 1)*AesalonPageSize);
	
	/* +1 for the header. */
	AI_InformerData.shmHeader->zonePageOffset =
		AI_InformerData.shmHeader->zoneUsagePages + AI_InformerData.shmHeader->configDataSize + 1;
}

static void AI_SetupZone() {
	printf("Setting up new zone . . .\n");
	printf("\tActive zone count:    %i\n", AI_InformerData.shmHeader->zoneCount);
	printf("\tAllocated zone count: %i\n", AI_InformerData.shmHeader->zonesAllocated);
	/* Check if more memory is required. */
	while(AI_InformerData.shmHeader->zoneCount >= AI_InformerData.shmHeader->zonesAllocated) {
		/* Allocate more memory. */
		sem_wait(&AI_InformerData.shmHeader->resizeSemaphore);
		
		if(AI_InformerData.shmHeader->zoneCount >= AI_InformerData.shmHeader->zonesAllocated) {
			AI_InformerData.shmHeader->shmSize += AI_InformerData.shmHeader->zoneSize;
			
			AI_InformerData.shmHeader->zonesAllocated ++;
			
			ftruncate(AI_InformerData.shmFd, AI_InformerData.shmHeader->shmSize * AesalonPageSize);
		}
		
		sem_post(&AI_InformerData.shmHeader->resizeSemaphore);
	}
	
	uint32_t i;
	for(i = 0; i < AI_InformerData.shmHeader->zonesAllocated; i ++) {
		int available = AI_ZoneAvailable(i);
		if(available) {
			printf("Zone %i is available!\n", i);
		}
		else {
			printf("Zone %i is not available.\n", i);
		}
		if(available) break;
	}
	if(i == AI_InformerData.shmHeader->zonesAllocated) {
		/* Something went pretty seriously wrong. Perhaps another target jumped in and took the spot first? */
		printf("Something very wrong occurred. Trying again . . .\n");
		AI_SetupZone();
	}
	AI_MarkZone(i);
	AI_Zone = mmap(NULL,
		AI_InformerData.shmHeader->zoneSize*AesalonPageSize,
		PROT_READ | PROT_WRITE, MAP_SHARED, AI_InformerData.shmFd,
		(AI_InformerData.shmHeader->zonePageOffset + i*AI_InformerData.shmHeader->zoneSize)*AesalonPageSize);
	
	((ZoneHeader_t *)AI_Zone)->head = ((ZoneHeader_t *)AI_Zone)->tail = ZoneDataOffset;
	((ZoneHeader_t *)AI_Zone)->overflow = 0;
	((ZoneHeader_t *)AI_Zone)->processID = getpid();
	((ZoneHeader_t *)AI_Zone)->threadID = pthread_self();
	
	sem_init(&((ZoneHeader_t *)AI_Zone)->packetSemaphore, 1, 0);
	sem_init(&((ZoneHeader_t *)AI_Zone)->overflowSemaphore, 1, 0);
	
	printf("Set up zone properly . . .\n");
}

static int AI_ZoneAvailable(uint32_t id) {
	uint32_t byteOffset = id / 8;
	uint32_t bitOffset = id % 8;
	uint32_t mask = 0x01;
	return !(AI_InformerData.zoneUseData[byteOffset] & (mask << bitOffset));
}

static void AI_MarkZone(uint32_t id) {
	uint32_t byteOffset = id / 8;
	uint32_t bitOffset = id % 8;
	uint32_t mask = 0x01;
	AI_InformerData.zoneUseData[byteOffset] |= (mask << bitOffset);
	AI_InformerData.shmHeader->zoneCount ++;
}

static void AI_ClearZone(uint32_t id) {
	uint32_t byteOffset = id / 8;
	uint32_t bitOffset = id % 8;
	uint32_t mask = 0x01;
	AI_InformerData.zoneUseData[byteOffset] &= ~(mask << bitOffset);
	AI_InformerData.shmHeader->zoneCount --;
}

static uint32_t AI_RemainingSpace() {
	ZoneHeader_t *header = (ZoneHeader_t *)AI_Zone;
	if(header->head <= header->tail) {
		return ((AI_InformerData.shmHeader->zoneSize*AesalonPageSize) - ZoneDataOffset)
			- (header->tail - header->head);
	}
	else {
		return header->head - ZoneDataOffset - header->tail;
	}
}

static void *AI_ReserveSpace(uint32_t amount) {
	uint32_t remaining = AI_RemainingSpace();
	ZoneHeader_t *header = (ZoneHeader_t *)AI_Zone;
	/*uint32_t zoneDataSize = (AI_InformerData.shmHeader->zoneSize*AesalonPageSize) - ZoneDataOffset;*/
	if(remaining < amount) {
		header->overflow = amount - remaining;
		sem_wait(&header->overflowSemaphore);
	}
	
	/* If the head is less than (or equal to) the tail, then the used memory
		is in one contiguous chunk, and the buffer has not wrapped yet. */
	if(header->tail <= header->head) {
/*		if(header->head + amount >= zoneDataSize) {
			header->gapSize = (amount + header->head) - zoneDataSize;
			amount += header->gapSize;
			header->head = ZoneDataOffset;
		}*/
		
		header->tail += amount;
		return &AI_Zone[header->tail-amount];
	}
	else {
		return NULL;
	}
}

void AI_StartPacket(ModuleID moduleID) {
	if(AI_Zone == NULL) AI_SetupZone();
	AI_ZonePacket = AI_ReserveSpace(sizeof(SHMPacketHeader_t));
	AI_ZonePacket->packetSize = 0;
	AI_ZonePacket->moduleID = moduleID;
}

void AC_EXPORT *AI_PacketSpace(uint32_t size) {
	AI_ZonePacket->packetSize += size;
	return AI_ReserveSpace(size);
}

void AC_EXPORT AI_EndPacket() {
	printf("packet size: %i\n", AI_ZonePacket->packetSize);
	AI_ZonePacket = NULL;
	
	ZoneHeader_t *header = (ZoneHeader_t *)AI_Zone;
	
	sem_post(&header->packetSemaphore);
	
	sem_post(&AI_InformerData.shmHeader->packetSemaphore);
}

uint64_t AI_Timestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return (t.tv_sec * 1000000000) + t.tv_nsec;
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
	return strcmp(s, "false") != 0;
}

pthread_t *AI_TargetThreadList(int *size) {
	if(size == NULL) return NULL;
	*size = AI_InformerData.threadCount;
	
	return AI_InformerData.threadList;
}

short AI_CollectionStatus() {
	if(AI_InformerData.threadList == NULL) return 0;
	pthread_t self = pthread_self();
	int i = 0;
	while(i < AI_InformerData.monitorThreadListSize) {
		if(pthread_equal(self, AI_InformerData.monitorThreadList[i])) return 0;
		i ++;
	}
	
	return 1;
}

void AI_StopCollection(pthread_t tid) {
	if(AI_InformerData.monitorThreadListSize < AesalonInformerMonitorThreadListSize) {
		AI_InformerData.monitorThreadList[AI_InformerData.monitorThreadListSize] = tid;
		AI_InformerData.monitorThreadListSize ++;
	}
	else {
		fprintf(stderr,
			"Too many threads in monitor thread list, output data will be corrupted with Aesalon"
			"collection-thread data.\n");
		fprintf(stderr, "Increasing threadListSize in build/config may be a good idea.\n");
	}
}

void AI_ContinueCollection(pthread_t tid) {
	int i = 0;
	while(i < AI_InformerData.monitorThreadListSize) {
		if(pthread_equal(AI_InformerData.monitorThreadList[i], tid)) {
			AI_InformerData.monitorThreadListSize --;
			AI_InformerData.monitorThreadList[i] =
				AI_InformerData.monitorThreadList[AI_InformerData.monitorThreadListSize-1];
			break;
		}
		i ++;
	}
}

