#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/timerfd.h>
#include <pthread.h>

#include "collector/Interface.h"

static int AC_mmapFd, AC_heartbeatFd;
static uint8_t *AC_memory;
static AC_MemoryMapHeader *AC_header;
static uint8_t AC_heartbeatStatus;
static pthread_t AC_heartbeatThread;

/* Function prototypes for internally-used, non-exposed functions. */
static int AC_remainingSpace();
static void AC_writeData(void *data, size_t size);
static void *AC_sendHeartbeats(void *unused);

void AC_CONSTRUCTOR AC_constructor() {
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
	
	/* NOTE: TFD_CLOEXEC was introduced in Linux 2.6.27; timerfd_create() is Linux-specific. */
	if((AC_heartbeatFd = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC)) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	struct itimerspec its;
	
	its.it_interval.tv_sec = 0;
	/* 10,000,000 nanoseconds is 1/100th of a second. E.g. 100 heartbeats per second. */
	its.it_interval.tv_nsec = 10000000;
	
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 10000000;
	
	timerfd_settime(AC_heartbeatFd, 0, &its, NULL);
	
	pthread_create(&AC_heartbeatThread, NULL, AC_sendHeartbeats, NULL);
}

void AC_DESTRUCTOR AC_destructor() {
	AC_heartbeatStatus = 0;
	pthread_join(AC_heartbeatThread, NULL);
	
	char filename[64];
	sprintf(filename, "AC-%i", getpid());
	shm_unlink(filename);
}

uint16_t AC_registerModuleInternal(const char *name) {
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
	if(packet->dataSize) AC_writeData(packet->data, packet->dataSize);
	
	sem_post(&AC_header->dataEndSemaphore);
	sem_post(&AC_header->dataSempahore);
	
	int value;
	sem_getvalue(&AC_header->dataSempahore, &value);
}

void *AC_sendHeartbeats(void *unused) {
	AC_heartbeatStatus = 1;
	while(AC_heartbeatStatus == 1) {
		uint64_t exp;
		read(AC_heartbeatFd, &exp, sizeof(exp));
		
		AC_DataPacket packet;
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = 0;
		packet.dataSize = 0;
		packet.data = NULL;
		
		AC_writePacket(&packet);
	}
	return NULL;
}

AC_Timestamp AC_timestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((AC_Timestamp)t.tv_sec * 1000000000) + t.tv_nsec;
}

uint8_t AC_hasCollectionBegun() {
	return AC_header->mainReached;
}

AC_Address AC_EXPORT AC_libraryOffset(const char *name) {
	int fd = open("/proc/self/maps", O_RDONLY);
	
	if(!fd) {
		printf("open() failed: %s\n", strerror(errno));
		return 0;
	}
	
	char buffer[256];
	int found = 0;
	AC_Address address = 0;
	int ret = 1;
	
	while(ret > 0) {
		char c = 0;
		int pos = 0;
		while((ret = read(fd, &c, sizeof(c))) && c != '\n') buffer[pos++] = c;
		buffer[pos] = 0;
		/*printf("Trying line \"%s\"\n", buffer);*/
		
		char mode[128], path[128];
		sscanf(buffer, "%lx-%*x %s %*s %*s %*s %s", &address, mode, path);
		
		if(strcmp(mode, "r-xp")) {
			continue;
		}
		if(!strncmp(strrchr(path, '/')+1, name, strlen(name))) {
			close(fd);
			return address;
		}
	}
	
	close(fd);
	return 0;
}

char *AC_configurationString(const char *module, const char *name) {
	char envName[256];
	snprintf(envName, 256, "ACM_%s_%s", module, name);
	return getenv(envName);
}

int AC_configurationInt(const char *module, const char *name) {
	char envName[256];
	snprintf(envName, 256, "ACM_%s_%s", module, name);
	char *envContent = getenv(envName);
	if(envContent == NULL) return 0;
	int content;
	sscanf(envContent, "%i", &content);
	return content;
}

int AC_configurationBool(const char *module, const char *name) {
	char envName[256];
	snprintf(envName, 256, "ACM_%s_%s", module, name);
	char *envContent = getenv(envName);
	if(envContent == NULL) return 0;
	else if(!strcmp(envContent, "false")) return 0;
	else if(!strcmp(envContent, "true")) return 1;
	return 0;
}
