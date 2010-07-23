#define AesalonCollectorModule

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "collectorInterface/Interface.h"

timer_t SendTimer;
struct sigevent SendTimerType;

void AesalonCpuTimeCollectorSendTime(union sigval unused) {
	if(AesalonCollectionStatus() == 0) return;
	DataPacket packet;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	
	uint64_t value = (ru.ru_utime.tv_sec * 1000000000) + (ru.ru_utime.tv_usec * 1000);
	
	/*struct timespec t;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	uint64_t value = (t.tv_sec * 1000000000) + t.tv_nsec;*/
	packet.data = &value;
	packet.dataSize = sizeof(value);
	AesalonCollectorSendPacket(&packet);
}

void __attribute__((constructor)) AesalonCpuTimeCollectorInitialize() {
	AesalonCollectorRegisterModule("cpuTime");
	
	SendTimerType.sigev_notify = SIGEV_THREAD;
	SendTimerType.sigev_notify_function = AesalonCpuTimeCollectorSendTime;
	
	if(timer_create(CLOCK_REALTIME, &SendTimerType, &SendTimer) == -1) {
		printf("Failed to create timer . . .\n");
		return;
	}
	
	const char **config = AesalonCollectorConfig("cpuTime");
	
	struct itimerspec its;
	
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 25000000;
	
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 1;
	
	timer_settime(SendTimer, 0, &its, NULL);
}

