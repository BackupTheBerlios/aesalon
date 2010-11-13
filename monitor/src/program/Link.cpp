/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Link.cpp

*/

#include "program/Link.h"

namespace Monitor {
namespace Program {

Link::Link(SharedMemory *sharedMemory) : m_sharedMemory(sharedMemory) {

}

Link::~Link() {
	
}

void *Link::run(void *voidInstance) {
	Link *instance = reinterpret_cast<Link *>(voidInstance);
	
	while(true) {
		instance->m_sharedMemory->wait();
	}
	
	return NULL;
}

Packet *Link::nextPacket() {
	
	return NULL;
}

} // namespace Program
} // namespace Monitor
