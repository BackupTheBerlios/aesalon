#define AesalonCollectorImplementation

#include <pthread.h>

#include "Interface.h"

void __attribute__((constructor)) AesalonCollectorConstructor() {
	
}

void __attribute__((destructor)) AesalonCollectorDestructor() {
	
}

void AesalonCollectorRegisterModule(uint16_t *id) {
	
}

void AesalonCollectorFillPacket(DataPacket *packet) {
	packet->dataSource.thread = pthread_self();
	packet->dataSource.timestamp = AesalonCollectorGetTimestamp();
}

void AesalonCollectorSendPacket(DataPacket *packet) {
	
}

uint64_t AesalonCollectorGetTimestamp() {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((uint64_t)t.tv_sec * 1000000000) + t.tv_nsec;
}
