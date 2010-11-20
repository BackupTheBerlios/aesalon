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

#include "informer/Informer.h"
#include "common/Config.h"
#include "common/ConductorPacket.h"

void __attribute__((constructor)) AI_Construct() {
	printf("**** Constructing Informer . . .\n");
	
	if(SharedMemory.data == NULL) AI_CreateSHM();
}

void __attribute__((destructor)) AI_Destruct() {
	/*printf("Destructing Informer . . .\n");*/
	if(SharedMemory.data) {
		munmap(SharedMemory.data, SharedMemory.size);
		SharedMemory.data = NULL, SharedMemory.size = 0;
	}
}

void AI_CreateSHM() {
	char shmName[256] = {0};
	
	SharedMemory.processHash = AI_Timestamp();
	sprintf(shmName, "AI-%lx", SharedMemory.processHash);
	
	int32_t shmSize = AI_ConfigurationLong("informer:shmSize");
	/* If shmSize is not specified, then set it to the compile-time size. */
	if(shmSize == -1) shmSize = AesalonDefaultShmSize;
	/* Turn the size into kilobytes . . . */
	SharedMemory.size = shmSize * 1024;
	
	SharedMemory.fd = shm_open(shmName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(SharedMemory.fd, SharedMemory.size);
	SharedMemory.data = mmap(NULL, SharedMemory.size, PROT_READ | PROT_WRITE, MAP_SHARED, SharedMemory.fd, 0);
	
	SharedMemory.header = (SharedMemoryHeader *)SharedMemory.data;
	
	sem_init(&SharedMemory.header->packetSemaphore, 1, 0);
	sem_init(&SharedMemory.header->overflowSemaphore, 1, 0);
	sem_init(&SharedMemory.header->sendSemaphore, 1, 0);
	
	
	SharedMemory.header->size = shmSize;
	SharedMemory.header->dataOffset =
		SharedMemory.header->dataStart =
		SharedMemory.header->dataEnd =
			sizeof(SharedMemoryHeader) + 16;
	
	int conductorFd = AI_ConfigurationLong("::conductorFd");
	
	uint8_t header = ConductorPacket_NewSHM;
	
	write(conductorFd, &header, sizeof(header));
	uint16_t length = strlen(shmName) + 1;
	write(conductorFd, &length, sizeof(length));
	write(conductorFd, shmName, length);
}

uint32_t AI_RemainingSpace() {
	if(SharedMemory.header->dataStart <= SharedMemory.header->dataEnd) {
		return (SharedMemory.header->size - SharedMemory.header->dataOffset)
			- (SharedMemory.header->dataEnd - SharedMemory.header->dataStart);
	}
	else {
		/*return (SharedMemory.header->size - SharedMemory.header->dataOffset) - 
			(SharedMemory.header->size - SharedMemory.header->dataStart + SharedMemory.header->dataEnd);*/
		return SharedMemory.header->dataStart - SharedMemory.header->dataOffset - SharedMemory.header->dataEnd;
	}
}

void AI_WriteData(void *data, size_t length) {
	memcpy(SharedMemory.data + SharedMemory.header->dataStart, data, length);
	SharedMemory.header->dataEnd += length;
}

void AI_SendPacket(Packet *packet) {
	printf("Sending packet . . .\n");
	if(packet == NULL) {
		sem_post(&SharedMemory.header->packetSemaphore);
		return;
	}
	
	packet->sourceHash = (SharedMemory.processHash ^ pthread_self());
	
	uint32_t size = sizeof(packet->sourceHash) + sizeof(packet->usedSize) + packet->usedSize;
	printf("Waiting for %u to become > %u . . .\n", AI_RemainingSpace, size);
	while(AI_RemainingSpace() < size) {
		SharedMemory.header->overflow = 1;
		printf("Waiting upon overflow semaphore . . .\n");
		printf("\tRemaining: %u Size: %u\n", AI_RemainingSpace(), size);
		sem_wait(&SharedMemory.header->overflowSemaphore);
	}
	SharedMemory.header->overflow = 0;
	
	printf("\twriting data . . .\n");
	AI_WriteData(&packet->sourceHash, sizeof(packet->sourceHash));
	AI_WriteData(&packet->usedSize, sizeof(packet->usedSize));
	AI_WriteData(packet->data, packet->usedSize);
	
	printf("\tposting semaphores . . .\n");
	sem_post(&SharedMemory.header->packetSemaphore);
	sem_post(&SharedMemory.header->sendSemaphore);
}

uint64_t AI_Timestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return (t.tv_sec * 1000000000) + t.tv_nsec;
}

const char *AI_ConfigurationString(const char *name) {
	char realname[256] = {"AC_"};
	int i = 0;
	while(name[i]) {
		if(name[i] == ':') realname[i+3] = '_';
		else realname[i+3] = name[i];
		i ++;
	}
	
	return getenv(realname);
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
	return s == NULL || s[0] == 0 || !strcmp(s, "false");
}

void AI_AppendUint64(Packet *packet, uint64_t value) {
	memcpy(packet->data + packet->usedSize, &value, sizeof(value));
	packet->usedSize += sizeof(value);
}

pid_t fork() {
	pid_t (*realFork)();
	
	*(void **)(&realFork) = dlsym(RTLD_NEXT, "fork");
	
	pid_t value = realFork();
	if(value == 0) AI_CreateSHM();
	
	return value;
}
