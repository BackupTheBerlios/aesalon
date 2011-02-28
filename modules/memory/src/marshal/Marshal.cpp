#include "Marshal.h"
#include "util/MessageSystem.h"
#include "storage/Mempool.h"

InstantiateMarshal(MemoryMarshal)

MemoryMarshal::MemoryMarshal() {
	
}

MemoryMarshal::~MemoryMarshal() {
	
}

Comm::Packet *MemoryMarshal::marshal(Comm::Packet *packet) {
	Message(Debug, "MemoryMarshal: marshalling packet . . .");
	
	return packet;
}
