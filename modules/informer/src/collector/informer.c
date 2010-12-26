#define AC_INFORMER

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
#include "common/PacketEncoding.h"

static InformerData AI_InformerData;

void __attribute__((constructor)) AI_Construct() {
	printf("[AI] **** Constructing Informer . . .\n");
	
	pid_t pid = getpid();
	
	uint32_t pathHash = 0;
	
	/* TODO: use CRC32 for this. */
	
	
	
	AI_InformerData.processID = pathHash ^ pid;
}

void __attribute__((destructor)) AI_Destruct() {
	printf("[AI] Destructing Informer . . .\n");
}

void AI_CreateSHM() {
	char shmName[256] = {0};
	
	InformerData.SharedMemory.processHash = AI_Timestamp();
	sprintf(shmName, "AI-%lx", InformerData.SharedMemory.processHash);
	
	int32_t shmSize = AI_ConfigurationLong("informer:shmSize");
	/* If shmSize is not specified, then set it to the compile-time size. */
	if(shmSize == -1) shmSize = AesalonDefaultShmSize;
	/* Turn the size into kilobytes . . . */
	InformerData.SharedMemory.size = shmSize * 1024;
	printf("[AI] Size of SHM is: %i\n", InformerData.SharedMemory.size);
	
	InformerData.SharedMemory.fd = shm_open(shmName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(InformerData.SharedMemory.fd, InformerData.SharedMemory.size);
	InformerData.SharedMemory.data =
		mmap(NULL, InformerData.SharedMemory.size, PROT_READ | PROT_WRITE, MAP_SHARED, InformerData.SharedMemory.fd, 0);
	
	InformerData.SharedMemory.header = (SharedMemoryHeader *)InformerData.SharedMemory.data;
	
	sem_init(&InformerData.SharedMemory.header->packetSemaphore, 1, 0);
	sem_init(&InformerData.SharedMemory.header->overflowSemaphore, 1, 0);
	sem_init(&InformerData.SharedMemory.header->sendSemaphore, 1, 1);
	sem_init(&InformerData.SharedMemory.header->readSemaphore, 1, 1);
	
	InformerData.SharedMemory.header->size = InformerData.SharedMemory.size;
	InformerData.SharedMemory.header->dataOffset =
		InformerData.SharedMemory.header->dataStart =
		InformerData.SharedMemory.header->dataEnd =
			SharedMemoryDataOffset;
	
	int conductorFd = AI_ConfigurationLong("::conductorFd");
	
	uint8_t header = ConductorPacket_NewSHM;
	
	write(conductorFd, &header, sizeof(header));
	uint16_t length = strlen(shmName) + 1;
	write(conductorFd, &length, sizeof(length));
	write(conductorFd, shmName, length);
}

uint32_t AI_RemainingSpace() {
	if(InformerData.SharedMemory.header->dataStart <= InformerData.SharedMemory.header->dataEnd) {
		return (InformerData.SharedMemory.header->size - InformerData.SharedMemory.header->dataOffset)
			- (InformerData.SharedMemory.header->dataEnd - InformerData.SharedMemory.header->dataStart);
	}
	else {
		/*return (InformerData.SharedMemory.header->size - InformerData.SharedMemory.header->dataOffset) - 
			(InformerData.SharedMemory.header->size - InformerData.SharedMemory.header->dataStart +
InformerData.SharedMemory.header->dataEnd);*/
		return InformerData.SharedMemory.header->dataStart - InformerData.SharedMemory.header->dataOffset -
InformerData.SharedMemory.header->dataEnd;
	}
}

void AI_WriteData(void *data, size_t length) {
	printf("[AI] Writing %lu bytes . . .\n", length);
	/* If dataStart <= dataEnd, then the used memory is a contigious chunk. */
	if(InformerData.SharedMemory.header->dataStart <= InformerData.SharedMemory.header->dataEnd) {
		/* Two possible scenarios: the data fits on the end . . . */
		if(length < (InformerData.SharedMemory.header->size  - InformerData.SharedMemory.header->dataEnd)) {
			printf("[AI] Normal case, writing memory in a single chunk.\n");
			printf("[AI] Offset: %u\n", InformerData.SharedMemory.header->dataEnd);
			memcpy(InformerData.SharedMemory.data + InformerData.SharedMemory.header->dataEnd, data, length);
			InformerData.SharedMemory.header->dataEnd += length;
		}
		/* And the data does not fit on the end. */
		else {
			printf("[AI] Special case, writing memory in two chunks . . .\n");
			size_t over = length - (InformerData.SharedMemory.header->size - InformerData.SharedMemory.header->dataEnd);
			size_t under = length - over;
			
			memcpy(InformerData.SharedMemory.data + InformerData.SharedMemory.header->dataEnd, data, under);
			
			memcpy(InformerData.SharedMemory.data + InformerData.SharedMemory.header->dataOffset, data + under, over);
			
			InformerData.SharedMemory.header->dataEnd = InformerData.SharedMemory.header->dataOffset + over;
		}
	}
	/* Else the used memory is in two separate chunks. */
	else {
		memcpy(InformerData.SharedMemory.data + InformerData.SharedMemory.header->dataEnd, data, length);
		InformerData.SharedMemory.header->dataEnd += length;
	}
}

void AI_SendPacket(Packet *packet) {
	if(InformerData.SharedMemory.data == NULL) return;
	sem_wait(&InformerData.SharedMemory.header->sendSemaphore);
	if(packet != NULL) {
		packet->sourceHash = (InformerData.SharedMemory.processHash ^ pthread_self());
		
		uint32_t size = sizeof(packet->sourceHash) + sizeof(packet->usedSize) + packet->usedSize;
		while(AI_RemainingSpace() < size) {
			InformerData.SharedMemory.header->overflow = 1;
			printf("[AI] **** Overflow!\n");
			sem_wait(&InformerData.SharedMemory.header->overflowSemaphore);
		}
		InformerData.SharedMemory.header->overflow = 0;
		
		AI_WriteData(&packet->sourceHash, sizeof(packet->sourceHash));
		printf("[AI] Data size: %u\n", packet->usedSize);
		AI_WriteData(&packet->usedSize, sizeof(packet->usedSize));
		AI_WriteData(packet->data, packet->usedSize);
	}
	sem_post(&InformerData.SharedMemory.header->packetSemaphore);
	int value;
	sem_getvalue(&InformerData.SharedMemory.header->packetSemaphore, &value);
	printf("[AI] value of packetSemaphore: %i\n", value);
	sem_post(&InformerData.SharedMemory.header->sendSemaphore);
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
