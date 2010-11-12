/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/ProcessMonitor.h

*/

#ifndef AesalonMonitor_Program_ProcessMonitor_H
#define AesalonMonitor_Program_ProcessMonitor_H

#include <sys/types.h>

#include "SharedMemory.h"

namespace Monitor {
namespace Program {

class ProcessMonitor {
private:
	pid_t m_pid;
	SharedMemory *m_sharedMemory;
public:
	ProcessMonitor(pid_t pid);
	~ProcessMonitor();
};

} // namespace Program
} // namespace Monitor

#endif
