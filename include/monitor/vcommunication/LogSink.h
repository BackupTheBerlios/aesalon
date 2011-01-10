/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/vcommunication/LogSink.h

*/

#ifndef AesalonMonitor_VCommunication_LogSink_H
#define AesalonMonitor_VCommunication_LogSink_H

#include <semaphore.h>

#include "DataSink.h"

namespace Monitor {
namespace VCommunication {

class LogSink : public DataSink {
private:
	int m_fd;
	sem_t m_logLock;
public:
	LogSink();
	~LogSink();
	
	/** Sink a packet into a log file.
	*/
	virtual void sinkPacket(Packet *packet);
};

} // namespace VCommunication
} // namespace Monitor

#endif
