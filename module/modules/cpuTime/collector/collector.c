#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <pthread.h>
#include <unistd.h>
#include "collector/Interface.h"

AC_moduleDefinition;

int ACM_timerFd;
pthread_t ACM_threadID;
int ACM_running;

static void *ACM_sendTime(void *unused) {
	ACM_running = 1;
	while(ACM_running) {
		uint64_t exp;
		read(ACM_timerFd, &exp, sizeof(exp));
		if(!AC_hasCollectionBegun()) continue;
	
		AC_DataPacket packet;
		struct rusage ru;
		getrusage(RUSAGE_SELF, &ru);
		
		uint64_t value = (ru.ru_utime.tv_sec * 1000000000) + (ru.ru_utime.tv_usec * 1000);
		
		/*struct tms times_value;
		times(&times_value);
		
		uint64_t value = times_value.tms_utime + times_value.tms_stime;
		value *= 10000000;
		
		printf("value: %lu\n", value);*/
		
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = AC_moduleID();
		packet.data = &value;
		packet.dataSize = sizeof(value);
		/*printf("Packet: timestamp=%lu, value=%lu\n", packet.dataSource.timestamp, value);*/
		AC_writePacket(&packet);
	}
	return NULL;
}

void __attribute__((constructor)) AC_constructor() {
	/* NOTE: TFD_CLOEXEC was introduced in Linux 2.6.27; timerfd_create() is Linux-specific. */
	if((ACM_timerFd = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC)) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	struct itimerspec its;
	
	int interval = AC_configurationInt("cpuTime", "interval");
	if(interval == 0) {
		/* The default interval is 10 times per second, or every 100 ms. */
		interval = 100000;
	}
	interval *= 1000;
	printf("interval: %i\n", interval);
	
	its.it_interval.tv_sec = interval / 1000000000;
	its.it_interval.tv_nsec = interval % 1000000000;
	
	its.it_value.tv_sec = interval / 1000000000;
	its.it_value.tv_nsec = interval % 1000000000;
	
	timerfd_settime(ACM_timerFd, 0, &its, NULL);
	
	pthread_create(&ACM_threadID, NULL, ACM_sendTime, NULL);
	
	AC_registerModule("cpuTime");
}

void __attribute__((destructor)) AC_destructor() {
	ACM_running = 0;
	pthread_join(ACM_threadID, NULL);
}
