/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/ProcessMonitor.cpp

*/

#include "program/ProcessMonitor.h"
#include "Coordinator.h"
#include "common/StringTo.h"
#include "common/StreamAsString.h"

namespace Monitor {
namespace Program {

ProcessMonitor::ProcessMonitor(pid_t pid) : m_pid(pid) {
	m_sharedMemory = new SharedMemory(Common::StreamAsString() << "aesalon-" << pid,
		Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("shmSize")));
}

ProcessMonitor::~ProcessMonitor() {
	delete m_sharedMemory;
}

} // namespace Program
} // namespace Monitor
