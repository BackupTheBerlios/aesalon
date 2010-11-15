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

Link::Link(std::string name, uint32_t size) : m_sharedMemory(NULL) {
	m_sharedMemory = new SharedMemory(name, size);
}

Link::~Link() {
	std::cout << "Link destructing . . ." << std::endl;
}

void Link::listen() {
	std::cout << "Link: creating thread . . ." << std::endl;
	pthread_create(&m_threadID, NULL, run, NULL);
}

void Link::terminate() {
	std::cout << "Link::terminate() called . . .\n";
	
	if(m_sharedMemory != NULL) {
		std::cout << "m_sharedMemory is not NULL, sending termination notice . . ." << std::endl;
		m_sharedMemory->notifyTermination();
	}
	
	std::cout << "Link::terminate(): joining thread . . ." << std::endl;
	pthread_join(m_threadID, NULL);
}

void *Link::run(void *voidInstance) {
	std::cout << "Link::run() . . ." << std::endl;
	Link *instance = reinterpret_cast<Link *>(voidInstance);
	
	Packet *packet = NULL;
	
	while((packet = instance->m_sharedMemory->readNext()) != NULL) {
		std::cout << "Received packet!" << std::endl;
	}
	
	std::cout << "Link::run() read loop ended. NULL packet recieved." << std::endl;
	
	delete instance->m_sharedMemory;
	instance->m_sharedMemory = NULL;
	
	return NULL;
}

} // namespace Program
} // namespace Monitor
