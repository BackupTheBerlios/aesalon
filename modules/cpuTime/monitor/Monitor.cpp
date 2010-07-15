#include <iostream>

#include "Monitor.h"

extern "C" {

MonitorInterface *AesalonMonitorCreateInstance() {
	return new CpuTimeMonitor();
}

} // extern "C"

CpuTimeMonitor::CpuTimeMonitor() {
}

CpuTimeMonitor::~CpuTimeMonitor() {
}

DataPacket *CpuTimeMonitor::handlePacket(DataPacket *packet) {
	
	return packet;
}
