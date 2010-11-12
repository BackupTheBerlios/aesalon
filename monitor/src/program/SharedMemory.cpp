/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/SharedMemory.cpp

*/

#include <sys/mman.h>
#include <fcntl.h>

#include "program/SharedMemory.h"
#include "common/AssertionException.h"

namespace Monitor {
namespace Program {

SharedMemory::SharedMemory(std::string identifier, uint32_t size) : m_identifier(identifier) {
	if(size == 0 || (size % 4) != 0)
		throw Common::AssertionException("Size of shared memory must be a nonzero multiple of four.");
	
	m_fd = shm_open(identifier.c_str(), O_RDWR, 0);
	
	m_memory = static_cast<uint8_t *>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
	
	// Yes, the reinterpret_cast<> is required. static_cast<> does not work for this.
	m_header = reinterpret_cast<SharedMemoryHeader *>(m_memory);
}

SharedMemory::~SharedMemory() {
	shm_unlink(m_identifier.c_str());
}

} // namespace Program
} // namespace Monitor
