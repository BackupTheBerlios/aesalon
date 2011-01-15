#include <iostream>

#include "CpuTimeMarshaller.h"

CpuTimeMarshaller::~CpuTimeMarshaller() {

}

void CpuTimeMarshaller::marhsall(Monitor::VCommunication::DataSink *dataSink, Common::VPacket *packet) {
	std::cout << "Sinking packet . . ." << std::endl;
	dataSink->sinkPacket(packet);
}

Common::MarshallerInterface *AM_InstantiateMarshaller() {
	std::cout << "Creating CpuTimeMarshaller . . ." << std::endl;
	return new CpuTimeMarshaller();
}
