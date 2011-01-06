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
#include <string.h>

#ifndef TFD_CLOEXEC
	#error "TFD_CLOEXEC must be defined; this is a Linux-specific feature available in 2.6.27 and later."
#endif

#include "informer/Informer.h"

static int isRunning = 0;
static int timerFd;
static pthread_t threadID;
static ModuleID moduleID;

static void *sendTime(void *unused) {
	isRunning = 1;
	while(isRunning) {
		uint64_t exp;
		read(timerFd, &exp, sizeof(exp));
		uint8_t buffer[64];
		
		struct rusage ru;
		getrusage(RUSAGE_SELF, &ru);
		
		uint64_t value = (ru.ru_utime.tv_sec * 1000000000) + (ru.ru_utime.tv_usec * 1000);
		
		AI_StartPacket(moduleID);
		
		void *packet = AI_PacketSpace(sizeof(value));
		memcpy(packet, &value, sizeof(value));
		
		AI_EndPacket();
	}
	return NULL;
}

void __attribute__((constructor)) AM_Construct() {
	/* Just in case, construct the Informer. */
	AI_Construct();
	
	/* Retrieve the module ID# . . . */
	moduleID = AI_ConfigurationLong("cpuTime:moduleID");
	
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
	
	int conductorFd = AI_ConfigurationLong("::conductorFd");
}

void __attribute__((destructor)) AM_Destruct() {
	isRunning = 0;
	pthread_join(threadID, NULL);
}
