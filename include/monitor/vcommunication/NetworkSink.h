/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/vcommunication/NetworkSink.h

*/

#ifndef AesalonMonitor_VCommunication_NetworkSink_H
#define AesalonMonitor_VCommunication_NetworkSink_H

#include "DataSink.h"

namespace Monitor {
namespace VCommunication {

class NetworkSink : public DataSink {
public:
	NetworkSink();
	~NetworkSink();
	
	/** Sink a packet across a network socket.
	*/
	virtual void sinkPacket(Packet *packet);
};

} // namespace VCommunication
} // namespace Monitor

#endif
