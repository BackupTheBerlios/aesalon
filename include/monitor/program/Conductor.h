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

#include "module/List.h"
#include "SharedMemory.h"
#include "vcommunication/DataSink.h"

namespace Monitor {
namespace Program {

class Conductor {
private:
	SharedMemory *m_sharedMemory;
	VCommunication::DataSink *m_dataSink;
public:
	Conductor(Monitor::Program::SharedMemory *sharedMemory, VCommunication::DataSink *dataSink);
	~Conductor();
	
	void run(Module::List *moduleList);
};

} // namespace Program
} // namespace Monitor

#endif
