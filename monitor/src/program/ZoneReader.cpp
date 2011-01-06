/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/ZoneReader.cpp

*/

#include "program/ZoneReader.h"

namespace Monitor {
namespace Program {

ZoneReader::ZoneReader(SharedMemory *sharedMemory, Module::List *moduleList) :
	m_sharedMemory(sharedMemory), m_moduleList(moduleList) {
	
}

ZoneReader::~ZoneReader() {
	
}

void ZoneReader::start() {
	m_threadID = pthread_self();
	run(this);
}

void ZoneReader::startInThread() {
	pthread_create(&m_threadID, NULL, run, this);
}

void ZoneReader::join() {
	pthread_join(m_threadID);
}

void *ZoneReader::run(void *voidInstance) {
	ZoneReader *instance = static_cast<ZoneReader *>(voidInstance);
	while(true) {
	
	}
	return NULL;
}

} // namespace Program
} // namespace Monitor
