/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/vcommunication/LogSink.cpp

*/

#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

#include "vcommunication/LogSink.h"

#include "Coordinator.h"
#include "common/PathSanitizer.h"

namespace Monitor {
namespace VCommunication {

LogSink::LogSink() {
	std::string filename = Common::PathSanitizer::sanitize(Coordinator::instance()->vault()->get("logFile"));
	
	std::cout << "LogSink: \"" << filename << "\"\n";
	
	if(filename == "") {
		m_fd = -1;
	}
	else {
		m_fd = open(filename.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	
		sem_init(&m_logLock, 0, 1);
	}
}

LogSink::~LogSink() {
	if(m_fd != -1) {
		sem_destroy(&m_logLock);
		close(m_fd);
	}
}

void LogSink::sinkPacket(Common::VPacket *packet) {
	if(m_fd == -1) return;
	
	sem_wait(&m_logLock);
	
	ModuleID moduleID = packet->moduleID();
	write(m_fd, &moduleID, sizeof(moduleID));
	
	uint32_t processID = packet->processID();
	write(m_fd, &processID, sizeof(processID));
	
	uint32_t threadID = packet->threadID();
	write(m_fd, &threadID, sizeof(threadID));
	
	uint32_t dataSize = packet->dataSize();
	write(m_fd, &dataSize, sizeof(dataSize));
	
	write(m_fd, packet->data(), dataSize);
	
	sem_post(&m_logLock);
}

} // namespace VCommunication
} // namespace Monitor
