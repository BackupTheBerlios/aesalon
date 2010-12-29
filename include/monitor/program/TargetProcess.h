/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/TargetProcess.h

*/

#ifndef AesalonMonitor_Program_TargetProcess_H
#define AesalonMonitor_Program_TargetProcess_H

#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>

#include "SMS.h"

namespace Monitor {
namespace Program {

class Conductor;

class TargetProcess {
private:
	Conductor *m_conductor;
	pid_t m_pid;
	pthread_t m_threadID;
	
	uint64_t m_processID;
	SMS *m_controlSMS;
	
	static void *monitor(void *voidInstance);
public:
	TargetProcess(Conductor *conductor, pid_t pid);
	~TargetProcess();
	
	void startMonitoring();
};

} // namespace Program
} // namespace Monitor

#endif
