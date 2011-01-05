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

typedef struct Module_t Module_t;

struct Module_t {
	const char *name;
	uint16_t moduleID;
};

typedef struct Zone_t Zone_t;
struct Zone_t {
	int offset;
	Zone_t *next;
};

typedef struct InformerData_t InformerData_t;
struct InformerData_t {
	int initialized;
	uint64_t processID;
	
	struct Module_t moduleList[AesalonInformerModuleListSize];
	int moduleListSize;
	
	pthread_t monitorThreadList[AesalonInformerMonitorThreadListSize];
	int monitorThreadListSize;
	
	pthread_t threadList[AesalonInformerThreadListSize];
	int threadListSize;
	
	int shmFd;
};

static InformerData_t AI_InformerData;

/** Interally-used function; creates a shared memory segment.
	@param id The ID# of the SHM.
	@param size The size, in kilobytes, of the SHM.
	@return A pointer to the new SHM instance.
*/
void AI_OpenSHM(const char *name);

/* ------------------------------------------------------------------ */

void __attribute__((constructor)) AI_Construct() {
	/* By default, .initialized will be set to 0 (AI_InformerData is a global). */
	if(AI_InformerData.initialized) return;
	AI_InformerData.initialized = 1;
	
	printf("[AI] **** Constructing Informer . . .\n");
	
	memset(AI_InformerData.threadList, 0, sizeof(pthread_t)*AesalonInformerThreadListSize);
	
	AI_StopCollection(pthread_self());
	
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
	
	printf("Path hash: %lx\n", pathHash);
	
	pathHash &= ~0xffff;
	
	AI_InformerData.processID = pathHash ^ pid;
	
	const char *shmName = getenv("AesalonSHMName");
	if(shmName == NULL) {
		fprintf(stderr, "[aesalon] AesalonSHMName not set, aborting.\n");
		exit(1);
	}
	
	AI_OpenSHM("");
	
	AI_ContinueCollection(pthread_self());
}

void __attribute__((destructor)) AI_Destruct() {
	printf("[AI] Destructing Informer . . .\n");
}

void AI_OpenSHM(const char *name) {
	printf("[AI] Size of SHM is: %i\n", size);
	
	AI_InformerData.shmFd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
	AI_InformerData.shm.data = mmap(NULL, AesalonSHMHeaderSize, PROT_READ | PROT_WRITE, MAP_SHARED,
		AI_InformerData.shm.fd, 0);
}

uint32_t AI_RemainingSpace() {
	if(shm->header->dataStart <= shm->header->dataEnd) {
		return (shm->header->size - shm->header->dataOffset)
			- (shm->header->dataEnd - shm->header->dataStart);
	}
	else {
		return shm->header->dataStart - shm->header->dataOffset - shm->header->dataEnd;
	}
}

void AI_WriteData(struct SHM_t *shm, void *data, size_t length) {
	printf("[AI] Writing %lu bytes . . .\n", length);
	/* If dataStart <= dataEnd, then the used memory is a contigious chunk. */
	if(shm->header->dataStart <= shm->header->dataEnd) {
		/* Two possible scenarios: the data fits on the end . . . */
		if(length < (shm->header->size - shm->header->dataEnd)) {
			printf("[AI] Normal case, writing memory in a single chunk.\n");
			printf("[AI] Offset: %u\n", shm->header->dataEnd);
			memcpy(shm->data + shm->header->dataEnd, data, length);
			shm->header->dataEnd += length;
		}
		/* And the data does not fit on the end. */
		else {
			printf("[AI] Special case, writing memory in two chunks . . .\n");
			size_t over = length - (shm->header->size - shm->header->dataEnd);
			size_t under = length - over;
			
			memcpy(shm->data + shm->header->dataEnd, data, under);
			
			memcpy(shm->data + shm->header->dataOffset, data + under, over);
			
			shm->header->dataEnd = shm->header->dataOffset + over;
		}
	}
	/* Else the used memory is in two separate chunks. */
	else {
		memcpy(shm->data + shm->header->dataEnd, data, length);
		shm->header->dataEnd += length;
	}
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

pthread_t *AI_TargetThreadList(int *size) {
	if(size == NULL) return NULL;
	*size = AI_InformerData.threadListSize;
	
	return AI_InformerData.threadList;
}

short AI_CollectionStatus() {
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
		fprintf(stderr, "Too many threads in monitor thread list, output data will be corrupted with Aesalon data.\n");
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

