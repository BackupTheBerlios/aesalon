#define AesalonCollectorModule

#include <stdio.h>
#include "collectorInterface/Interface.h"

void __attribute__((constructor)) AesalonCpuTimeCollectorInitialize() {
	AesalonCollectorRegisterModule("cpuTime");
}

