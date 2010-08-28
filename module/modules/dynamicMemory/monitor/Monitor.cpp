#include <iostream>

#include "Monitor.h"

extern "C" {

MonitorInterface *AesalonMonitorCreateInstance() {
	return new DynamicMemoryMonitor();
}

} // extern "C"

DynamicMemoryMonitor::DynamicMemoryMonitor() {
}

DynamicMemoryMonitor::~DynamicMemoryMonitor() {
}

DataPacket *DynamicMemoryMonitor::handlePacket(DataPacket *packet) {
	
	return packet;
}
