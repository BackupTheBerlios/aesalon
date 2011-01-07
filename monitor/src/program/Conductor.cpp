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
#include "Coordinator.h"
#include "common/StringTo.h"
#include "common/Config.h"
#include "common/StreamAsString.h"
#include "program/ZoneReader.h"

namespace Monitor {
namespace Program {

Conductor::Conductor(SharedMemory *sharedMemory) : m_sharedMemory(sharedMemory) {
	
}

Conductor::~Conductor() {
	
}

void Conductor::run(Module::List *moduleList) {
	std::list<ZoneReader *> readerList;
	int readerCount = Common::StringTo<int>(Coordinator::instance()->vault()->get("zoneReaders"));
	for(int i = 1; i < readerCount; i ++) {
		ZoneReader *reader = new ZoneReader(m_sharedMemory, moduleList);
		reader->start();
		readerList.push_back(reader);
	}
	ZoneReader reader(m_sharedMemory, moduleList);
	reader.startInThread();
	std::cout << "exiting Conductor::run() . . ." << std::endl;
}

} // namespace Program
} // namespace Monitor
