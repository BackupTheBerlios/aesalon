#include <iostream>

#include "cpuTime.h"

CpuTime::~CpuTime() {

}

Packet *CpuTime::polish(Packet *packet) {
	std::cout << "CpuTime: Asked to polish packet . . ." << std::endl;
	return packet;
}

Common::PolisherInterface *AP_Instantiate() {
	return new CpuTime();
}
