#include "Marshal.h"
#include "util/MessageSystem.h"

InstantiateMarshal(CpuTimeMarshal)

CpuTimeMarshal::CpuTimeMarshal() {
	
}

CpuTimeMarshal::~CpuTimeMarshal() {
	
}

Comm::Packet *CpuTimeMarshal::marshal(Comm::Packet *packet) {
	Message(Debug, "CpuTimeMarshal: marshalling packet . . .");
	return packet;
}
