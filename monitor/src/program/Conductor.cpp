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

namespace Monitor {
namespace Program {

void Conductor::monitor() {
	uint8_t header;
	
	while(true) {
		int ret = read(m_readFd, &header, sizeof(header));
		/* If ret == 0, then EOF was reached. */
		if(ret == 0) break;
		/* If the read was interrupted by a signal, continue. */
		else if(ret == -1 && errno == EINTR) continue;
		
		if(header == ConductorPacket_NewSHM) {
			newLink();
		}
		else if(header == ConductorPacket_ModuleLoaded) {
			loadModule();
		}
	}
}

void Conductor::newLink() {
	uint16_t length;
	read(m_readFd, &length, sizeof(length));
	char name[256];
	read(m_readFd, name, length);
	
	/*std::cout << "New process with PID " << pid << " recognized." << std::endl;*/
	
	std::cout << "NewLink packet received, name is \"" << name << "\"\n";
}

void Conductor::loadModule() {
	uint16_t length;
	read(m_readFd, &length, sizeof(length));
	char name[256];
	read(m_readFd, name, length);
	
	
}

} // namespace Program
} // namespace Monitor
