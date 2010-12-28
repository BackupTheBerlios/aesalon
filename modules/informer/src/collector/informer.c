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

struct Module_t {
	const char *name;
	LinkPropagationMode linkMode;
	uint16_t moduleID;
};

struct SMS_t {
	int fd;
	uint8_t *data;
	uint32_t size;
	
	/** The SMS ID is an XOR of pthread_self() and the process identifier.
		(use 0 for pthread_self() for non-thread-specific SMSes).
	*/
	uint64_t smsID;
	
	SharedMemoryHeader *header;
};

struct InformerData_t {
	uint64_t processID;
	
	struct Module_t moduleList[AesalonInformerModuleListSize];
	int moduleListSize;
	
	struct SMS_t smsList[AesalonInformerSMSListSize];
	int smsListSize;
	
	pthread_t monitorThreadList[AesalonInformerMonitorThreadListSize];
	int monitorThreadListSize;
	
	pthread_t threadList[AesalonInformerThreadListSize];
	int threadListSize;
	
	int initialized;
};


static struct InformerData_t AI_InformerData;

/** Interally-used function; creates a shared memory segment.
	@param id The ID# of the SMS.
	@param size The size, in kilobytes, of the SMS.
	@return A pointer to the new SMS instance.
*/
static struct SMS_t *AI_CreateSMS(uint64_t id, uint32_t size);

/** Internally-used function; retrieves a SMS_t instance given its ID.
	@param id The ID of the SMS to retrieve.
	@return A pointer to the MS instance, or NULL if it was not found.
*/
static struct SMS_t *AI_GetSMS(uint64_t id);

static int AI_CompareSMS(const void *key, const void *object);

/* ------------------------------------------------------------------ */

void __attribute__((constructor)) AI_Construct() {
	/* By default, .initialized will be set to 0 (AI_InformerData is a global). */
	if(AI_InformerData.initialized) return;
	AI_InformerData.initialized = 1;
	
	printf("[AI] **** Constructing Informer . . .\n");
	
	memset(AI_InformerData.smsList, 0, sizeof(struct SMS_t)*AesalonInformerSMSListSize);
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
	
	AI_CreateSMS(AI_InformerData.processID, 4);
	
	AI_ContinueCollection(pthread_self());
}

void __attribute__((destructor)) AI_Destruct() {
	printf("[AI] Destructing Informer . . .\n");
}

static struct SMS_t *AI_CreateSMS(uint64_t id, uint32_t size) {
	char smsName[256] = {0};
	int i = 0;
	
	if(AI_InformerData.smsListSize == AesalonInformerSMSListSize) {
		fprintf(stderr, "Cannot create SMS: SMS list size exceeded.\n");
		return NULL;
	}
	
	sprintf(smsName, "/AI-%lx", id);
	
	struct SMS_t *sms = NULL;
	
	for(; i < AI_InformerData.smsListSize; i ++) {
		if(AI_InformerData.smsList[i].smsID > id) break;
	}
	
	if(i != AI_InformerData.smsListSize) {
		memmove(&AI_InformerData.smsList[i+1], &AI_InformerData.smsList[i],
			sizeof(struct SMS_t)*(AI_InformerData.smsListSize-i));
	}
	sms = &AI_InformerData.smsList[i];
	
	size *= 1024;
	
	sms->size = size;
	
	printf("[AI] Size of SMS is: %i\n", size);
	
	sms->fd = shm_open(smsName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(sms->fd, size);
	sms->data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, sms->fd, 0);
	
	sms->header = (SharedMemoryHeader *)sms->data;
	
	sem_init(&sms->header->packetSemaphore, 1, 0);
	sem_init(&sms->header->overflowSemaphore, 1, 0);
	sem_init(&sms->header->sendSemaphore, 1, 1);
	sem_init(&sms->header->readSemaphore, 1, 1);
	
	sms->header->size = sms->size;
	sms->header->dataOffset = sms->header->dataStart = sms->header->dataEnd =
			SharedMemoryDataOffset;
	
	return sms;
}

static struct SMS_t *AI_GetSMS(uint64_t id) {
	return bsearch(&id, AI_InformerData.smsList, AI_InformerData.smsListSize, sizeof(struct SMS_t *), AI_CompareSMS);
}

static int AI_CompareSMS(const void *key, const void *voidObject) {
	uint64_t id = *(uint64_t *)key;
	struct SMS_t *object = (struct SMS_t *)voidObject;
	if(id < object->smsID) return -1;
	else if(id > object->smsID) return 1;
	return 0;
}

uint32_t AI_RemainingSpace(struct SMS_t *sms) {
	if(sms->header->dataStart <= sms->header->dataEnd) {
		return (sms->header->size - sms->header->dataOffset)
			- (sms->header->dataEnd - sms->header->dataStart);
	}
	else {
		return sms->header->dataStart - sms->header->dataOffset - sms->header->dataEnd;
	}
}

void AI_WriteData(struct SMS_t *sms, void *data, size_t length) {
	printf("[AI] Writing %lu bytes . . .\n", length);
	/* If dataStart <= dataEnd, then the used memory is a contigious chunk. */
	if(sms->header->dataStart <= sms->header->dataEnd) {
		/* Two possible scenarios: the data fits on the end . . . */
		if(length < (sms->header->size - sms->header->dataEnd)) {
			printf("[AI] Normal case, writing memory in a single chunk.\n");
			printf("[AI] Offset: %u\n", sms->header->dataEnd);
			memcpy(sms->data + sms->header->dataEnd, data, length);
			sms->header->dataEnd += length;
		}
		/* And the data does not fit on the end. */
		else {
			printf("[AI] Special case, writing memory in two chunks . . .\n");
			size_t over = length - (sms->header->size - sms->header->dataEnd);
			size_t under = length - over;
			
			memcpy(sms->data + sms->header->dataEnd, data, under);
			
			memcpy(sms->data + sms->header->dataOffset, data + under, over);
			
			sms->header->dataEnd = sms->header->dataOffset + over;
		}
	}
	/* Else the used memory is in two separate chunks. */
	else {
		memcpy(sms->data + sms->header->dataEnd, data, length);
		sms->header->dataEnd += length;
	}
}

void AI_SendPacket(Packet *packet) {
	//if(InformerData.SharedMemory.data == NULL) return;
	struct SMS_t *sms = NULL;
	sem_wait(&sms->header->sendSemaphore);
	if(packet != NULL) {
		//packet->sourceHash = (InformerData.SharedMemory.processHash ^ pthread_self());
		
		uint32_t size = sizeof(packet->sourceHash) + sizeof(packet->usedSize) + packet->usedSize;
		while(AI_RemainingSpace(sms) < size) {
			sms->header->overflow = 1;
			printf("[AI] **** Overflow!\n");
			sem_wait(&sms->header->overflowSemaphore);
		}
		sms->header->overflow = 0;
		
		AI_WriteData(sms, &packet->sourceHash, sizeof(packet->sourceHash));
		printf("[AI] Data size: %u\n", packet->usedSize);
		AI_WriteData(sms, &packet->usedSize, sizeof(packet->usedSize));
		AI_WriteData(sms, packet->data, packet->usedSize);
	}
	sem_post(&sms->header->packetSemaphore);
	int value;
	sem_getvalue(&sms->header->packetSemaphore, &value);
	printf("[AI] value of packetSemaphore: %i\n", value);
	sem_post(&sms->header->sendSemaphore);
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

