#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include "collector/Interface.h"

AC_moduleDefinition;

static timer_t SendTimer;
static struct sigevent SendTimerType;

void ACM_sendTime(union sigval unused) {
	if(!AC_hasCollectionBegun()) return;
	
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

void __attribute__((constructor)) AC_constructor() {
	printf("Constructing cpuTime . . .\n");
	SendTimerType.sigev_notify = SIGEV_THREAD;
	SendTimerType.sigev_notify_function = ACM_sendTime;
	
	if(timer_create(CLOCK_REALTIME, &SendTimerType, &SendTimer) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	struct itimerspec its;
	
	int interval = AC_configurationInt("cpuTime", "interval");
	interval *= 1000;
	printf("interval: %i\n", interval);
	
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = interval;
	
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 1;
	
	timer_settime(SendTimer, 0, &its, NULL);
	
	printf("Constructed. Registering . . .\n");
	
	AC_registerModule("cpuTime");
	
	printf("Registered.\n");
}

void __attribute__((destructor)) AC_destructor() {
	union sigval unused;
	ACM_sendTime(unused);
}
