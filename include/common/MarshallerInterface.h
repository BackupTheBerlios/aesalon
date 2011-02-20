/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/MarshallerInterface.h

*/

#ifndef AesalonCommon_MarshallerInterface_H
#define AesalonCommon_MarshallerInterface_H

#include "VPacket.h"
#include "vcommunication/DataSink.h"

namespace Common {

class MarshallerInterface {
public:
	virtual ~MarshallerInterface() {}
	
	virtual void marhsall(Monitor::VCommunication::DataSink *dataSink, VPacket *packet) = 0;
};

} // namespace Common

#endif
