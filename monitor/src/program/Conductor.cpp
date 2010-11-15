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

namespace Monitor {
namespace Program {

Conductor::Conductor(int readFd) : m_readFd(readFd) {
	std::cout << "Conductor created." << std::endl;
}

Conductor::~Conductor() {
	std::cout << "Conductor destructing . . ." << std::endl;
	for(std::list<Link *>::iterator i = m_linkList.begin(); i != m_linkList.end(); ++ i) {
		delete (*i);
	}
	
	std::cout << "Conductor destructed" << std::endl;
}

void Conductor::monitor() {
	uint8_t header;
	uint32_t shmSize = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("informer:shmSize"));
	
	while(true) {
		std::cout << "Conductor: waiting for packet header." << std::endl;
		int ret = read(m_readFd, &header, sizeof(header));
		std::cout << "Received packet header." << std::endl;
		/* If ret == 0, then EOF was reached. */
		if(ret == 0) break;
		/* If the read was interrupted by a signal, continue. */
		else if(ret == -1 && errno == EINTR) continue;
		
		if(header == ConductorPacket_NewSHM) {
			Link *link = newLink(shmSize);
			m_linkList.push_back(link);
			link->listen();
		}
		else if(header == ConductorPacket_ModuleLoaded) {
			loadModule();
		}
	}
	std::cout << "Conductor::monitor finished!" << std::endl;
}

void Conductor::join() {
	for(std::list<Link *>::iterator i = m_linkList.begin(); i != m_linkList.end(); ++ i) {
		(*i)->terminate();
	}
}

Link *Conductor::newLink(uint32_t size) {
	uint16_t length;
	read(m_readFd, &length, sizeof(length));
	char name[256];
	read(m_readFd, name, length);
	
	/*std::cout << "New process with PID " << pid << " recognized." << std::endl;*/
	
	std::cout << "NewLink packet received, name is \"" << name << "\"\n";
	
	return new Link(name, size);
}

void Conductor::loadModule() {
	uint16_t length;
	read(m_readFd, &length, sizeof(length));
	char name[256];
	read(m_readFd, name, length);
}

} // namespace Program
} // namespace Monitor
