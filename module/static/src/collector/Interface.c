#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "Interface.h"

static int AC_mmapFd;
static uint8_t *AC_memory;
static AC_MemoryMapHeader *AC_header;

/* Function prototypes for internally-used, non-exposed functions. */
int AC_remainingSpace();
void AC_writeData(void *data, size_t size);

void __attribute__((constructor)) AC_constructor() {
	printf("Constructing module interface . . .\n");
	char filename[64];
	sprintf(filename, "AC-%i", getpid());
	
	AC_mmapFd = shm_open(filename, O_RDWR, 0);
	
	char *shmSizeEnv = getenv("AC_ShmSize");
	if(shmSizeEnv == NULL) {
		printf("AC_ShmSize is not set. Aborting.\n");
		exit(1);
	}
	int shmSize;
	sscanf(shmSizeEnv, "%i", &shmSize);
	
	AC_memory = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, AC_mmapFd, 0);
	
	AC_header = (AC_MemoryMapHeader *)AC_memory;
	
	printf("Module interface construction complete.\n");
}

void __attribute__((destructor)) AC_destructor() {
	char filename[64];
	sprintf(filename, "AC-%i", getpid());
	shm_unlink(filename);
}

uint16_t AC_registerModule(const char *name) {
	uint16_t id = ++AC_header->latestModule;
	
	printf("AC_RegisterModule: registering module %s as id %i.\n", name, id);
	
	AC_DataPacket packet;
	packet.dataSource.moduleID = 0;
	packet.dataSource.timestamp = AC_timestamp();
	packet.dataSize = strlen(name) + 1; /* Plus one for the NULL. */
	packet.data = (void *)name;
	AC_writePacket(&packet);
	return id;
}

int AC_remainingSpace() {
	int used = 0;
	
	if(AC_header->dataStart <= AC_header->dataEnd) {
		used = AC_header->dataEnd - AC_header->dataStart;
	}
	else {
		used = AC_header->dataSize - AC_header->dataStart;
		used += AC_header->dataEnd;
	}
	return (AC_header->dataSize - AC_header->dataOffset) - used;
}

void AC_writeData(void *data, size_t size) {
	/* If dataStart <= dataEnd, then the used memory is a contigious chunk. */
	if(AC_header->dataStart <= AC_header->dataEnd) {
		/* Two possible scenarios: the data fits on the end . . . */
		if(size < (AC_header->dataSize - AC_header->dataEnd)) {
			memcpy(AC_memory + AC_header->dataEnd, data, size);
			AC_header->dataEnd += size;
		}
		/* And the data does not fit on the end. */
		else {
			size_t over = size - (AC_header->dataSize - AC_header->dataEnd);
			size_t under = size - over;
			
			memcpy(AC_memory + AC_header->dataEnd, data, under);
			
			memcpy(AC_memory + AC_header->dataOffset, data + under, over);
			
			AC_header->dataEnd = AC_header->dataOffset + over;
		}
	}
	/* Else the used memory is in two separate chunks. */
	else {
		memcpy(AC_memory + AC_header->dataEnd, data, size);
		AC_header->dataEnd += size;
	}
}

void AC_writePacket(AC_DataPacket *packet) {
	sem_wait(&AC_header->dataEndSemaphore);
	
	int size = sizeof(packet->dataSource) + sizeof(packet->dataSize) + packet->dataSize;
	
	while(AC_remainingSpace() < size) {
		AC_header->dataOverflow = 1;
		sem_wait(&AC_header->dataOverflowSemaphore);
	}
	AC_header->dataOverflow = 0;
	
	AC_writeData(&packet->dataSource, sizeof(packet->dataSource));
	AC_writeData(&packet->dataSize, sizeof(packet->dataSize));
	AC_writeData(packet->data, packet->dataSize);
	
	sem_post(&AC_header->dataEndSemaphore);
	sem_post(&AC_header->dataSempahore);
	
	int value;
	sem_getvalue(&AC_header->dataSempahore, &value);
}

uint64_t AC_timestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((uint64_t)t.tv_sec * 1000000000) + t.tv_nsec;
}

uint8_t AC_hasCollectionBegun() {
	return AC_header->mainReached;
}
