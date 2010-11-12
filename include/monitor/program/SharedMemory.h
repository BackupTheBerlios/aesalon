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
	uint8_t *m_memory;
	SharedMemoryHeader *m_header;
	std::string m_identifier;
public:
	SharedMemory(std::string identifier, uint32_t size);
	~SharedMemory();
};

} // namespace Program
} // namespace Monitor

#endif
