/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/SharedMemory.h

*/

#ifndef AesalonMonitor_Program_SharedMemory_H
#define AesalonMonitor_Program_SharedMemory_H

#include <string>

#include "common/SharedMemoryHeader.h"

namespace Monitor {
namespace Program {

class SharedMemory {
private:
	int m_fd;
	std::string m_shmName;
	
	SharedMemoryHeader_t *m_header;
public:
	SharedMemory();
	~SharedMemory();
private:
	void setupHeader();
	void setupConfiguration();
};

} // namespace Program
} // namespace Monitor

#endif
