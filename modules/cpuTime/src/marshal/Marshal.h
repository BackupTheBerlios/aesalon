#ifndef AesalonModule_cpuTime_Marshal_H
#define AesalonModule_cpuTime_Marshal_H

#include "marshal/Interface.h"

class CpuTimeMarshal : public Marshal::Interface {
public:
	CpuTimeMarshal();
	virtual ~CpuTimeMarshal();
	
	virtual Comm::Packet *marshal(Comm::Packet *packet);
};

#endif
