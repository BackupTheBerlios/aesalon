/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Link.cpp

*/

#include <iostream>

#include "program/Link.h"

namespace Monitor {
namespace Program {

Link::Link(std::string name, uint32_t size, Module::List *moduleList)
	: m_sharedMemory(NULL), m_moduleList(moduleList) {
	
	m_sharedMemory = new SharedMemory(name, size);
}

Link::~Link() {
	terminate();
}

void Link::listen() {
	pthread_create(&m_threadID, NULL, run, this);
}

void Link::terminate() {
	if(m_sharedMemory != NULL) {
		m_sharedMemory->notifyTermination();
	}
	
	pthread_join(m_threadID, NULL);
}

void *Link::run(void *voidInstance) {
	Link *instance = reinterpret_cast<Link *>(voidInstance);
	
	Packet *packet = NULL;
	
	while((packet = instance->m_sharedMemory->readNext()) != NULL) {
		/*std::cout << "\tReceived packet!" << std::endl;
		std::cout << "first eight bytes translate to: " << ((uint64_t *)packet->data)[0] << std::endl;
		std::cout << "second eight bytes translate to: " << ((uint64_t *)packet->data)[1] << std::endl;*/
		
		delete[] packet->data;
		delete packet;
	}
	
	std::cout << "[monitor] **** Link terminating!" << std::endl;
	
	delete instance->m_sharedMemory;
	instance->m_sharedMemory = NULL;
	
	return NULL;
}

} // namespace Program
} // namespace Monitor
