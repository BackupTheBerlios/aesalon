#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "informer/Informer.h"
#include "common/Config.h"
#include "common/ConductorPacket.h"

void __attribute__((constructor)) AI_Construct() {
	printf("Constructing Informer . . .\n");
	char shmName[256] = {0};
	
	sprintf(shmName, "AI-%i", getpid());
	
	int32_t shmSize = AI_ConfigurationLong("informer:shmSize");
	/* If shmSize is not specified, then set it to the compile-time size. */
	if(shmSize == -1) shmSize = AesalonDefaultShmSize;
	/* Turn the size into kilobytes . . . */
	SharedMemory.size = shmSize * 1024;
	
	SharedMemory.fd = shm_open(shmName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(SharedMemory.fd, SharedMemory.size);
	SharedMemory.data = mmap(NULL, SharedMemory.size, PROT_READ | PROT_WRITE, MAP_SHARED, SharedMemory.fd, 0);
	
	int conductorFd = AI_ConfigurationLong("::conductorFd");
	pid_t pid = getpid();
	
	uint8_t header = ConductorPacket_NewProcess;
	
	write(conductorFd, &header, sizeof(header));
	write(conductorFd, &pid, sizeof(pid));
}

void __attribute__((destructor)) AI_Destruct() {
	munmap(SharedMemory.data, SharedMemory.size);
	printf("Destructing Informer . . .\n");
}

void AI_SendPacket(Packet *packet) {
	
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

int32_t AI_ConfigurationLong(const char *name) {
	const char *s = AI_ConfigurationString(name);
	if(s == NULL) return -1;
	int32_t value;
	sscanf(s, "%d", &value);
	return value;
}

int AI_ConfigurationBool(const char *name) {
	const char *s = AI_ConfigurationString(name);
	return s == NULL || s[0] == 0 || !strcmp(s, "false");
}
