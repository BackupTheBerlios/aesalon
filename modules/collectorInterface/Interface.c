#define AesalonCollectorImplementation

#include <pthread.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Interface.h"

void __attribute__((constructor)) AesalonCollectorConstructor() {
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
}

void __attribute__((destructor)) AesalonCollectorDestructor() {
	char filename[64];
	sprintf(filename, "AesalonCollector-%i", getpid());
	shm_unlink(filename);
}

void AesalonCollectorRegisterModule(uint16_t *id) {
	printf("AesalonCollectorRegisterModule called. ID is %p, content is %i.\n", id, *id);
}

void AesalonCollectorFillPacket(DataPacket *packet) {
	packet->dataSource.thread = pthread_self();
	packet->dataSource.timestamp = AesalonCollectorGetTimestamp();
}

void AesalonCollectorSendPacket(DataPacket *packet) {
	
}

uint64_t AesalonCollectorGetTimestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((uint64_t)t.tv_sec * 1000000000) + t.tv_nsec;
}

uint8_t AesalonCollectionStatus() {
	return AesalonMemoryMap.header->isMainReached;
}
