#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <search.h>
#include <errno.h>
#include <time.h>

#include "Interface.h"

/* Function prototypes for module FPs. */
void AC_SendPacket(AC_DataPacket *packet);
uint64_t AC_GetTimestamp();
uint8_t AC_Status();

/* Function prototypes for internally-used, non-exposed functions. */
int AC_RemainingSpace();
void AC_WriteData(void *data, size_t size);

struct hsearch_data AC_ModuleHash;
AC_Interface AC_globalInstance;

void __attribute__((constructor)) AC_Constructor() {
	char filename[64];
	sprintf(filename, "AC-%i", getpid());
	
	AC_globalInstance.mmap_fd = shm_open(filename, O_RDWR, 0);
	
	char *shmSizeEnv = getenv("AC_ShmSize");
	if(shmSizeEnv == NULL) {
		printf("AC_ShmSize is not set. Aborting.\n");
		exit(1);
	}
	int shmSize;
	sscanf(shmSizeEnv, "%i", &shmSize);
	
	AC_globalInstance.memory = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, AC_globalInstance.mmap_fd, 0);
	
	AC_globalInstance.header = (AC_MemoryMapHeader *)AC_globalInstance.memory;
	
	/* NOTE: this limits the maximum total number of modules to 64; probably not a good limitation to have. */
	hcreate_r(64, &AC_ModuleHash);
	
	AC_globalInstance.sendPacket = AC_SendPacket;
	AC_globalInstance.status = AC_Status;
	AC_globalInstance.getTimestamp = AC_GetTimestamp;
}

void __attribute__((destructor)) AC_Destructor() {
	char filename[64];
	sprintf(filename, "AC-%i", getpid());
	shm_unlink(filename);
}

AC_Interface AC_EXPORT *AC_GetInterface() {
	return &AC_globalInstance;
}

void AC_EXPORT *AC_GetModule(const char *name) {
	ENTRY e, *ep;
	e.key = (char *)name;
	if(hsearch_r(e, FIND, &ep, &AC_ModuleHash) == 0 || ep == NULL) {
		printf("AC_GetModule: Couldn't find module \"%s\": ", name);
		/* NOTE: hsearch_r() is not specified by the POSIX standard, and the
			ESRCH errno value is not specified for hsearch() by POSIX.1-2001.
			As such, checking for ESRCH is probably not very portable. */
		if(errno == ESRCH || ep == NULL) printf("No such item.\n");
		else printf("Unknown error.\n");
		return NULL;
	}
	return ep->data;
}

void AC_RegisterModule(AC_Module *module) {
	ENTRY e, *ep;
	
	e.key = (char *)module->name;
	e.data = module;
	
	if(hsearch_r(e, ENTER, &ep, &AC_ModuleHash) == 0) {
		printf("AC_RegisterModule: Failed to register module: ");
		if(errno == ENOMEM) printf("No space left in hash table.\n");
		else printf("Unknown error.\n");
		return;
	}
	
	module->id = ++AC_globalInstance.header->latestModule;
	
	printf("AC_RegisterModule: registering module %s as id %i.\n", module->name, module->id);
	AC_DataPacket packet;
	packet.dataSource.moduleID = 0;
	packet.dataSource.timestamp = AC_GetTimestamp();
	packet.dataSize = strlen(module->name) + 1; /* Plus one for the NULL. */
	packet.data = (void *)module->name;
	AC_SendPacket(&packet);
}

int AC_RemainingSpace() {
	int used = 0;
	
	if(AC_globalInstance.header->dataStart <= AC_globalInstance.header->dataEnd) {
		used = AC_globalInstance.header->dataEnd - AC_globalInstance.header->dataStart;
	}
	else {
		used = AC_globalInstance.header->dataSize - AC_globalInstance.header->dataStart;
		used += AC_globalInstance.header->dataEnd;
	}
	return (AC_globalInstance.header->dataSize - AC_globalInstance.header->dataOffset) - used;
}

void AC_WriteData(void *data, size_t size) {
	/* If dataStart <= dataEnd, then the used memory is a contigious chunk. */
	if(AC_globalInstance.header->dataStart <= AC_globalInstance.header->dataEnd) {
		/* Two possible scenarios: the data fits on the end . . . */
		if(size < (AC_globalInstance.header->dataSize - AC_globalInstance.header->dataEnd)) {
			memcpy(AC_globalInstance.memory + AC_globalInstance.header->dataEnd, data, size);
			AC_globalInstance.header->dataEnd += size;
		}
		/* And the data does not fit on the end. */
		else {
/*			printf("AC_WriteData(): overflow detected.\n");*/
			size_t over = size - (AC_globalInstance.header->dataSize - AC_globalInstance.header->dataEnd);
			size_t under = size - over;
/*			printf("\ttotal size:%i\n\tdataEnd:%i\n",
				AC_globalInstance.header->dataSize, AC_globalInstance.header->dataEnd);*/
/*			printf("\tdata size: %i\n\tover: %i\n\tunder:%i\n", size, over, under);*/
			
			memcpy(AC_globalInstance.memory + AC_globalInstance.header->dataEnd, data, under);
			
			memcpy(AC_globalInstance.memory + AC_globalInstance.header->dataOffset, data + under, over);
			
			AC_globalInstance.header->dataEnd = AC_globalInstance.header->dataOffset + over;
		}
	}
	/* Else the used memory is in two separate chunks. */
	else {
		memcpy(AC_globalInstance.memory + AC_globalInstance.header->dataEnd, data, size);
		AC_globalInstance.header->dataEnd += size;
	}
}

void AC_SendPacket(AC_DataPacket *packet) {
	sem_wait(&AC_globalInstance.header->dataEndSemaphore);
	
	int size = sizeof(packet->dataSource) + sizeof(packet->dataSize) + packet->dataSize;
	
	while(AC_RemainingSpace() < size) {
		AC_globalInstance.header->dataOverflow = 1;
		sem_wait(&AC_globalInstance.header->dataOverflowSemaphore);
	}
	AC_globalInstance.header->dataOverflow = 0;
	
	AC_WriteData(&packet->dataSource, sizeof(packet->dataSource));
	AC_WriteData(&packet->dataSize, sizeof(packet->dataSize));
	AC_WriteData(packet->data, packet->dataSize);
	
	sem_post(&AC_globalInstance.header->dataEndSemaphore);
	sem_post(&AC_globalInstance.header->dataSempahore);
	
	int value;
	sem_getvalue(&AC_globalInstance.header->dataSempahore, &value);
}

uint64_t AC_GetTimestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((uint64_t)t.tv_sec * 1000000000) + t.tv_nsec;
}

uint8_t AC_Status() {
	return AC_globalInstance.header->mainReached;
}
