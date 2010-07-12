#ifndef CpuTimeMonitor_H
#define CpuTimeMonitor_H

#include "moduleInterface/Interface.h"

class CpuTimeMonitor : public ModuleInterface {
public:
	CpuTimeMonitor();
	virtual ~CpuTimeMonitor();
private:
	
public:
	virtual DataPacket *handlePacket(DataPacket *packet);
};

#endif
