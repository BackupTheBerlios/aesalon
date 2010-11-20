#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <pthread.h>

#include "informer/Informer.h"
#include "common/PacketEncoding.h"

int isRunning = 0;
int timerFd;
pthread_t threadID;

static void *sendTime(void *unused) {
	isRunning = 1;
	while(isRunning) {
		uint64_t exp;
		read(timerFd, &exp, sizeof(exp));
		uint8_t buffer[64];
		
		Packet packet;
		packet.data = buffer;
		packet.dataSize = sizeof(buffer);
		packet.usedSize = 0;
		
		struct rusage ru;
		getrusage(RUSAGE_SELF, &ru);
		
		uint64_t value = (ru.ru_utime.tv_sec * 1000000000) + (ru.ru_utime.tv_usec * 1000);
		AI_AppendTimestamp(&packet);
		AI_AppendUint64(&packet, value);
		
		AI_SendPacket(&packet);
	}
	return NULL;
}

void __attribute__((constructor)) AM_Construct() {
	/* NOTE: TFD_CLOEXEC was introduced in Linux 2.6.27; timerfd_create() is Linux-specific. */
	if((timerFd = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC)) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	struct itimerspec its;
	
	int interval = AI_ConfigurationLong("cpuTime:interval");
	if(interval == -1) {
		/* The default interval is 10 times per second, or every 100 ms. */
		interval = 100000;
	}
	interval *= 1000;
	
	its.it_interval.tv_sec = interval / 1000000000;
	its.it_interval.tv_nsec = interval % 1000000000;
	
	its.it_value.tv_sec = interval / 1000000000;
	its.it_value.tv_nsec = interval % 1000000000;
	
	timerfd_settime(timerFd, 0, &its, NULL);
	
	pthread_create(&threadID, NULL, sendTime, NULL);

}

void __attribute__((destructor)) AM_Destruct() {
	isRunning = 0;
	pthread_join(threadID, NULL);
}
