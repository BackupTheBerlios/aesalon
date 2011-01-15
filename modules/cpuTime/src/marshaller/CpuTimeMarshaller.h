#ifndef CpuTime_Marshaller_H
#define CpuTime_Marshaller_H

#include "common/MarshallerInterface.h"

class CpuTimeMarshaller : public Common::MarshallerInterface {
public:
	virtual ~CpuTimeMarshaller();
	
	virtual void marhsall(Monitor::VCommunication::DataSink *dataSink, Common::VPacket *packet);
};

extern "C" {

Common::MarshallerInterface *AM_InstantiateMarshaller();

} // extern "C"

#endif
