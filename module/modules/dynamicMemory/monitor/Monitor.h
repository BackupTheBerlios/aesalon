#ifndef DynamicMemoryMonitor_H
#define DynamicMemoryMonitor_H

#include "monitor/Interface.h"

class DynamicMemoryMonitor : public MonitorInterface {
public:
	DynamicMemoryMonitor();
	virtual ~DynamicMemoryMonitor();
private:
	
public:
	virtual DataPacket *handlePacket(DataPacket *packet);
};

#endif
