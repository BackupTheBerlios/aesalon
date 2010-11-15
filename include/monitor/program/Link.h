/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/Link.h

*/

#ifndef AesalonMonitor_Program_Link_H
#define AesalonMonitor_Program_Link_H

#include <pthread.h>

#include "SharedMemory.h"

#include "common/Packet.h"

namespace Monitor {
namespace Program {

class Link {
private:
	SharedMemory *m_sharedMemory;
	pthread_t m_threadID;
public:
	Link(std::string name, uint32_t size);
	~Link();
	
	SharedMemory *sharedMemory() const { return m_sharedMemory; }
	
	void listen();
private:
	static void *run(void *voidInstance);
	Packet *nextPacket();
};

} // namespace Program
} // namespace Monitor

#endif
