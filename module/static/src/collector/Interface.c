#define AesalonCollectorImplementation

#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <search.h>
#include <errno.h>

#include "Interface.h"

struct hsearch_data AC_ModuleHash;

void __attribute__((constructor)) AC_Constructor() {
	char filename[64];
	sprintf(filename, "AesalonCollector-%i", getpid());
	
	AesalonMemoryMap.fd = shm_open(filename, O_RDWR, 0);
	
	char *shmSizeEnv = getenv("AesalonCollectorShmSize");
	if(shmSizeEnv == NULL) {
		printf("AesalonCollectorShmSize is not set. Aborting.\n");
		exit(1);
	}
	int shmSize;
	sscanf(shmSizeEnv, "%i", &shmSize);
	
	AesalonMemoryMap.memory = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, AesalonMemoryMap.fd, 0);
	
	AesalonMemoryMap.header = (MemoryMapHeader *)AesalonMemoryMap.memory;
	
	/* FIXME: this limits the maximum total number of modules to 64; probably not a good limitation to have. */
	hcreate_r(64, &AC_ModuleHash);
}

void __attribute__((destructor)) AC_Destructor() {
	char filename[64];
	sprintf(filename, "AesalonCollector-%i", getpid());
	shm_unlink(filename);
}

void AC_EXPORT * AC_GetModule(const char* name) {
	
}

void AC_RegisterModule(AC_Module *module) {
	ENTRY e, *ep;
	
	e.key = module->name;
	e.data = module;
	
	if(hsearch_r(e, ENTER, &ep, &AC_ModuleHash) == 0) {
		printf("AC_RegisterModule: failed to register module: ");
		if(errno == ENOMEM) printf("No space left in hash table.\n");
		else printf("Unknown error.\n");
		return;
	}
	
	module->id = ++AesalonMemoryMap.header->latestModule;
	
	printf("AC_RegisterModule: registering module %s as id %i.\n", module->name, module->id);
	AC_DataPacket packet;
	packet.dataSource.moduleID = 0;
	packet.dataSize = strlen(moduleName) + 1; /* Plus one for the NULL. */
	packet.data = (void *)moduleName;
	AesalonCollectorSendPacket(&packet);
}

int AesalonCollectorRemainingSpace() {
	int used = 0;
	
	if(AesalonMemoryMap.header->dataStart <= AesalonMemoryMap.header->dataEnd) {
		
		used = AesalonMemoryMap.header->dataEnd - AesalonMemoryMap.header->dataStart;
	}
	else {
		used = AesalonMemoryMap.header->dataSize - AesalonMemoryMap.header->dataStart;
		used += AesalonMemoryMap.header->dataEnd;
	}
	return (AesalonMemoryMap.header->dataSize - AesalonMemoryMap.header->dataOffset) - used;
}

void AesalonCollectorWriteData(void *data, int size) {
	int remaining = AesalonMemoryMap.header->dataSize - AesalonMemoryMap.header->dataEnd;
	if(remaining > size) {
		/* If this is a simple copy . . . */
		memcpy(AesalonMemoryMap.memory + AesalonMemoryMap.header->dataEnd, data, size);
		AesalonMemoryMap.header->dataEnd += size;
	}
	else {
		/* It's not a single copy. */
		/* First copy . . . */
		memcpy(AesalonMemoryMap.memory + AesalonMemoryMap.header->dataEnd, data, remaining);
		
		/* Second copy. */
		memcpy(AesalonMemoryMap.memory + AesalonMemoryMap.header->dataOffset, data + remaining, size - remaining);
	}
}

void AesalonCollectorSendPacket(DataPacket *packet) {
	AesalonCollectorFillPacket(packet);
	/*sem_wait(&AesalonMemoryMap.header->dataStartSemaphore);*/
	sem_wait(&AesalonMemoryMap.header->dataEndSemaphore);
	
	int size = sizeof(packet->dataSource) + sizeof(packet->dataSize) + packet->dataSize;
	
	while(AesalonCollectorRemainingSpace() < size) {
		AesalonMemoryMap.header->dataOverflow = 1;
		sem_wait(&AesalonMemoryMap.header->dataOverflowSemaphore);
	}
	AesalonMemoryMap.header->dataOverflow = 0;
	
	AesalonCollectorWriteData(&packet->dataSource, sizeof(packet->dataSource));
	AesalonCollectorWriteData(&packet->dataSize, sizeof(packet->dataSize));
	AesalonCollectorWriteData(packet->data, packet->dataSize);
	
	/*sem_post(&AesalonMemoryMap.header->dataStartSemaphore);*/
	sem_post(&AesalonMemoryMap.header->dataEndSemaphore);
	sem_post(&AesalonMemoryMap.header->dataSempahore);
	
	int value;
	sem_getvalue(&AesalonMemoryMap.header->dataSempahore, &value);
}

uint64_t AesalonCollectorGetTimestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((uint64_t)t.tv_sec * 1000000000) + t.tv_nsec;
}

uint8_t AesalonCollectionStatus() {
	return AesalonMemoryMap.header->mainReached;
}

const char **AesalonCollectorConfig(const char *module) {
	
	return NULL;
}
