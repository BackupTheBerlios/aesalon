/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/LogOutput.cpp
*/

#include <fcntl.h>
#include <errno.h>
#include <cstring>

#include "monitor/LogOutput.h"
#include "util/MessageSystem.h"

namespace Monitor {

LogOutput::LogOutput(std::string spec) {
	m_fd = open(spec.c_str(), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if(m_fd == -1) {
		Message(Fatal, "Could not open log file \"" << spec << "\": " << std::strerror(errno));
	}
	sem_init(&m_logSemaphore, 0, 1);
}

LogOutput::~LogOutput() {
	close(m_fd);
}

void LogOutput::output(Comm::Packet *packet) {
	sem_wait(&m_logSemaphore);
	
	write(m_fd, &packet->header(), sizeof(packet->header()));
	write(m_fd, packet->data(), packet->header().dataSize);
	
	sem_post(&m_logSemaphore);
}

} // namespace Monitor
