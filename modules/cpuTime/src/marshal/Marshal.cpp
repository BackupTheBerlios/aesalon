#include "Marshal.h"
#include "util/MessageSystem.h"
#include "storage/Mempool.h"

InstantiateMarshal(CpuTimeMarshal)

CpuTimeMarshal::CpuTimeMarshal() {
	
}

CpuTimeMarshal::~CpuTimeMarshal() {
	
}

Comm::Packet *CpuTimeMarshal::marshal(Comm::Packet *packet) {
	Message(Debug, "CpuTimeMarshal: marshalling packet . . .");
	
	return packet;
}
