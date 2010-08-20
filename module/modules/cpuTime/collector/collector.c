#define AesalonCollectorModule

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "collector/Interface.h"

timer_t SendTimer;
struct sigevent SendTimerType;
AC_Module *this;

void AC_SendTime(union sigval unused) {
	if(AC_GetInterface()->status() == 0) return;
	AC_DataPacket packet;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	
	uint64_t value = (ru.ru_utime.tv_sec * 1000000000) + (ru.ru_utime.tv_usec * 1000);
	
	/*struct timespec t;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	uint64_t value = (t.tv_sec * 1000000000) + t.tv_nsec;*/
	packet.dataSource.timestamp = AC_GetInterface()->getTimestamp();
	packet.dataSource.moduleID = this->id;
	packet.data = &value;
	packet.dataSize = sizeof(value);
	AC_GetInterface()->sendPacket(&packet);
}

void __attribute__((constructor)) AC_Constructor() {
	this = malloc(sizeof(AC_Module));
	this->name = "cpuTime";
	this->id = 0;
	this->modulePtr = NULL;
	AC_RegisterModule(this);
	
	SendTimerType.sigev_notify = SIGEV_THREAD;
	SendTimerType.sigev_notify_function = AC_SendTime;
	
	if(timer_create(CLOCK_REALTIME, &SendTimerType, &SendTimer) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	struct itimerspec its;
	
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 25000000;
	
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 1;
	
	timer_settime(SendTimer, 0, &its, NULL);
}

void __attribute__((constructor)) AC_Destructor() {
	union sigval unused;
	AC_SendTime(unused);
}
