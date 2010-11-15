/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/SharedMemory.cpp

*/

#include <iostream>

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
	std::cout << "Unlinking " << m_identifier << " . . ." << std::endl;
	shm_unlink(m_identifier.c_str());
}

void SharedMemory::wait() {
	std::cout << "Acquiring lock . . ." << std::endl;
	int result = 0;
	while((result = sem_wait(&m_header->packetSemaphore)) != 0) {
		
	}
	std::cout << "**** Lock acquired!" << std::endl;
}

Packet *SharedMemory::readNext() {
	std::cout << "reading next packet . . ." << std::endl;
	wait();
	/* If the data start is the same as the end, then a NULL packet has been
		sent -- otherwise known as the termination signal.
	*/
	if(m_header->dataStart == m_header->dataEnd) return NULL;
	return NULL;
}

void SharedMemory::notifyTermination() {
	/* Artificially create a termination signal by posting to the packet semaphore. */
	sem_post(&m_header->packetSemaphore);
}

} // namespace Program
} // namespace Monitor
