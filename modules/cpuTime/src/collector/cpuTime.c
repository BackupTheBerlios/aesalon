#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "informer/Informer.h"

static pthread_t AM_threadID;

static void *run(void *unused);

static void *run(void *unused) {
	ModuleID moduleID = AI_ConfigurationLong("cpuTime:moduleID");
	printf("moduleID: %i\n", moduleID);
	printf("*****\n");
	sleep(2);
	int64_t interval_us = AI_ConfigurationLong("cpuTime:interval");
	/* If the interval is not set, use the default of 1 ms. */
	if(interval_us == -1) interval_us = 1000;
	
	printf("interval_us: %li\n", interval_us);
	
	struct timespec interval;
	interval.tv_sec = interval_us / 1000000;
	interval.tv_nsec = (interval_us % 1000000)*1000;
	/* Use nanosleep() to sleep for the given period.
		NOTE: this may cause issues with systems that overuse signals . . .
	*/
	while(1) {
		struct timespec value;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &value);
		uint64_t timestamp = AI_Timestamp();
		
		AI_StartPacket(1);
		*(uint64_t *)AI_PacketSpace(8) = timestamp;
		*(uint64_t *)AI_PacketSpace(8) = value.tv_nsec + (value.tv_sec * 1000000000);
		AI_EndPacket();
		
		struct timespec remaining;
		int ret = nanosleep(&interval, &remaining);
		while(ret == -1 && errno == EINTR) {
			printf("Interrupted by signal!\n");
			ret = nanosleep(&remaining, &remaining);
		}
	}
	
	return NULL;
}

void __attribute__((constructor)) AC_EXPORT AM_Construct() {
	AI_Construct();
	
	pthread_create(&AM_threadID, NULL, run, NULL);
}

void __attribute__((destructor)) AC_EXPORT AM_Destruct() {
	
}
