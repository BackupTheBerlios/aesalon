/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/Conductor.h

*/

#ifndef AesalonMonitor_Program_Conductor_H
#define AesalonMonitor_Program_Conductor_H

#include <stdint.h>
#include <list>
#include <semaphore.h>

#include "Link.h"
#include "module/List.h"
#include "TargetProcess.h"

namespace Monitor {
namespace Program {

class Conductor {
private:
	typedef std::list<TargetProcess *> TargetList;
	TargetList m_targetList;
	sem_t m_targetSemaphore;
public:
	Conductor();
	~Conductor();
	
	void addTarget(pid_t targetPid);
	void removeTarget(TargetProcess *targetProcess);
	void run();
	
	pid_t createDaemon();
};

} // namespace Program
} // namespace Monitor

#endif
