#include "Polisher.h"

CreateInstantiationFunction(DynamicMemoryPolisher)

DynamicMemoryPolisher::DynamicMemoryPolisher() {

}

DynamicMemoryPolisher::~DynamicMemoryPolisher() {

}

DataPacket *DynamicMemoryPolisher::handlePacket(DataPacket *packet) {
	return packet;
}
