/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/vcommunication/LogSink.cpp

*/

#include <string>
#include <fcntl.h>

#include "vcommunication/LogSink.h"

#include "Coordinator.h"
#include "common/PathSanitizer.h"

namespace Monitor {
namespace VCommunication {

LogSink::LogSink() {
	std::string filename = Common::PathSanitizer::sanitize(Coordinator::instance()->vault()->get("logFile"));
	
	if(filename == "") {
		m_fd = -1;
	}
	else {
		m_fd = open(filename.c_str(), O_RDWR);
	
		sem_init(&m_logLock, 0, 1);
	}
}

LogSink::~LogSink() {
	if(m_fd != -1) {
		sem_destroy(&m_logLock);
		close(m_fd);
	}
}

void LogSink::sinkPacket(Packet *packet) {
	if(m_fd == -1) return;
	
	sem_wait(&m_logLock);
	
	
	
	sem_post(&m_logLock);
}

} // namespace VCommunication
} // namespace Monitor
