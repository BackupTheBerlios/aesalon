/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Conductor.cpp

*/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "program/Conductor.h"
#include "common/ConductorPacket.h"
#include "Coordinator.h"
#include "common/StringTo.h"
#include "common/Config.h"

namespace Monitor {
namespace Program {

Conductor::Conductor() {
	sem_init(&m_targetSemaphore, 0, 0);
}

Conductor::~Conductor() {
	
}

void Conductor::addTarget(pid_t targetPid) {
	std::cout << "Adding " << targetPid << " as target PID . . ." << std::endl;
	TargetProcess *tp = new TargetProcess(this, targetPid);
	
	m_targetList.push_back(tp);
	
	tp->startMonitoring();
}

void Conductor::removeTarget(TargetProcess *targetProcess) {
	m_targetList.remove(targetProcess);
	if(m_targetList.size() == 0) sem_post(&m_targetSemaphore);
}	

void Conductor::run() {
	sem_wait(&m_targetSemaphore);
	std::cout << "exiting Conductor::run() . . ." << std::endl;
}

pid_t Conductor::createDaemon() {
	pid_t childPid = fork();
	
	if(childPid == -1) {
		/* TODO: print error or do something. */
	}
	return childPid;
}

} // namespace Program
} // namespace Monitor
