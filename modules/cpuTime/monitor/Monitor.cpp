#include <iostream>

#include "Monitor.h"

extern "C" {

ModuleInterface *AesalonMonitorCreateInstance() {
	return new CpuTimeMonitor();
}

}

CpuTimeMonitor::CpuTimeMonitor() {
}

CpuTimeMonitor::~CpuTimeMonitor() {
}

DataPacket *CpuTimeMonitor::handlePacket(DataPacket *packet) {
	
	return packet;
}
