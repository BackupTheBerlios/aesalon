#ifndef AesalonModule_memory_Marshal_H
#define AesalonModule_memory_Marshal_H

#include "marshal/Interface.h"

class MemoryMarshal : public Marshal::Interface {
public:
	MemoryMarshal();
	virtual ~MemoryMarshal();
	
	virtual Comm::Packet *marshal(Comm::Packet *packet);
};

#endif
