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

#include "Link.h"

namespace Monitor {
namespace Program {

class Conductor {
private:
	int m_readFd;
	std::list<Link *> m_linkList;
public:
	Conductor(int readFd);
	~Conductor();
	
	void monitor();
	
	void join();
private:
	static void *run(void *voidInstance);
	Link *newLink(uint32_t size);
	void loadModule();
};

} // namespace Program
} // namespace Monitor

#endif
