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

Conductor::Conductor(int readFd) : m_readFd(readFd) {
	m_moduleList = new Module::List();
}

Conductor::~Conductor() {
	for(std::list<Link *>::iterator i = m_linkList.begin(); i != m_linkList.end(); ++ i) {
		delete (*i);
	}
	delete m_moduleList;
}

void Conductor::monitor() {
	uint8_t header;
	uint32_t shmSize = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("informer:smsSize"));
	if(shmSize == 0) shmSize = AesalonDefaultSMSSize;
	shmSize *= 1024;
	
	while(true) {
		int ret = read(m_readFd, &header, sizeof(header));
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
		else if(header == ConductorPacket_Fork) {
			handleFork();
		}
	}
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
	
	std::cout << "[monitor] NewLink packet received, name is \"" << name << "\"\n";
	
	return new Link(name, size, m_moduleList);
}

void Conductor::loadModule() {
	uint16_t length;
	read(m_readFd, &length, sizeof(length));
	char name[256];
	read(m_readFd, name, length);
	/*std::cout << "asked to load module \"" << name << "\"\n";*/
	Module::Module *module = new Module::Module(name);
	
	m_moduleList->addModule(module);
}

void Conductor::handleFork() {
	pid_t pid;
	read(m_readFd, &pid, sizeof(pid));
	std::cout << "[monitor] Target process forked; new pid is " << pid << " . . .\n";
	/* TODO: handle process forks properly (spawn off new monitor). */
}

} // namespace Program
} // namespace Monitor
