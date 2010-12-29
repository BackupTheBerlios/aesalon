/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/SMS.cpp

*/

#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "program/SMS.h"

namespace Monitor {
namespace Program {

SMS::SMS(uint64_t id, uint32_t size) : m_id(id), m_size(size) {
	char smsName[256];
	sprintf(smsName, "/AI-%lx", id);
	
	m_fd = shm_open(smsName, O_RDWR, 0);
	
	m_data = static_cast<uint8_t *>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
}

SMS::~SMS() {

}



} // namespace Program
} // namespace Monitor
