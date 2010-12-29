/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/TargetProcess.cpp

*/

#include <iostream>
#include <fcntl.h>
#include <stdio.h>

#include "program/TargetProcess.h"
#include "program/Conductor.h"
#include "program/SMS.h"
#include "common/Config.h"

namespace Monitor {
namespace Program {

TargetProcess::TargetProcess(Conductor *conductor, pid_t pid) : m_conductor(conductor), m_pid(pid) {

}

TargetProcess::~TargetProcess() {

}

void *TargetProcess::monitor(void *voidInstance) {
	std::cout << "TargetProcess::monitor() called." << std::endl;
	TargetProcess *instance = static_cast<TargetProcess *>(voidInstance);
	char filename[1024];
	
	std::cout << "instance->m_pid: " << instance->m_pid << std::endl;
	
	sprintf(filename, "/proc/%i/cmdline", instance->m_pid);
	
	int fd = open(filename, O_RDONLY);
	read(fd, filename, sizeof(filename));
	close(fd);
	
	uint64_t pathHash = 0;
	int c = 0;
	char *p = filename;
	while((c = (*p++))) {
		pathHash = c + (pathHash << 6) + (pathHash << 16) - pathHash;
	}
	
	pathHash &= ~0xffff;
	
	instance->m_processID = pathHash ^ instance->m_pid;
	
	instance->m_controlSMS = new SMS(instance->m_processID, AesalonControlSMSSize);
	
	while(true) {
		// Read packets from control SMS.
	}
	
	Conductor *conductor = instance->m_conductor;
	conductor->removeTarget(instance);
	
	delete instance;
	
	return NULL;
}

void TargetProcess::startMonitoring() {
	pthread_create(&m_threadID, NULL, monitor, this);
}

} // namespace Program
} // namespace Monitor
