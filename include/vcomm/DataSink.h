/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/vcomm/DataSink.h

*/

#ifndef AesalonVComm_DataSink_H
#define AesalonVComm_DataSink_H

#include "common/VPacket.h"

namespace VComm {

class DataSink {
public:
	/** "Sink" a packet, e.g. process it in whatever manner the implementation can.
		@param packet The packet to sink.
	*/
	virtual void sinkPacket(Common::VPacket *packet) = 0;
};

} // namespace VComm

#endif
