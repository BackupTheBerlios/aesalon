#include <iostream>

#include "Monitor.h"

extern "C" {

ModuleInterface *AesalonMonitorCreateInstance() {
	return new CpuTimeMonitor();
}

}

CpuTimeMonitor::CpuTimeMonitor() {
	std::cout << "CpuTimeMonitor instance created . . ." << std::endl;
}

CpuTimeMonitor::~CpuTimeMonitor() {
	std::cout << "CpuTimeMonitor instance destroyed . . ." << std::endl;
}

DataPacket *CpuTimeMonitor::handlePacket(DataPacket *packet) {
	std::cout << "CpuTimeMonitor handling packet . . ." << std::endl;
	return packet;
}
