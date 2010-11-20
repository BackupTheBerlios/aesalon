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
#include <string.h>

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

void SharedMemory::wait() {
	int result = 0;
	while((result = sem_wait(&m_header->packetSemaphore)) != 0) {
		std::cout << "semaphore error!" << std::endl;
	}
}

Packet *SharedMemory::readNext() {
	std::cout << "wait() called . . .\n";
	wait();
	/* If the data start is the same as the end, then a NULL packet has been
		sent -- otherwise known as the termination signal.
	*/
	if(m_header->dataStart == m_header->dataEnd) return NULL;
	
	Packet *packet = new Packet;
	
	sem_wait(&m_header->sendSemaphore);
	
	std::cout << "Reading packet . . ." << std::endl;
	readData(&packet->sourceHash, sizeof(packet->sourceHash));
	std::cout << "\tsourceHash: " << std::hex << packet->sourceHash << std::dec << std::endl;
	readData(&packet->usedSize, sizeof(packet->usedSize));
	std::cout << "\tusedSize: " << packet->usedSize << std::endl;
	packet->dataSize = packet->usedSize;
	packet->data = new uint8_t[packet->dataSize];
	readData(packet->data, packet->dataSize);
	
	if(m_header->overflow) {
		sem_post(&m_header->overflowSemaphore);
		m_header->overflow = 0;
	}
	
	sem_post(&m_header->sendSemaphore);
	
	return packet;
}

void SharedMemory::notifyTermination() {
	/* Artificially create a termination signal by posting to the packet semaphore. */
	sem_post(&m_header->packetSemaphore);
}

void SharedMemory::readData(void *buffer, size_t size) {
	if(m_header->dataStart + size >= m_header->size) {
		std::cout << "Reading overflow data (might blow up . . .)\n";
		int end_copy_size = (m_header->size - m_header->dataStart);
		memcpy(buffer, m_memory + m_header->dataStart, end_copy_size);
		
		memcpy((char *)buffer + end_copy_size, m_memory + m_header->dataOffset, size - end_copy_size);
		m_header->dataStart = m_header->dataOffset + size - end_copy_size;
	}
	else {
		memcpy(buffer, m_memory + m_header->dataStart, size);
		m_header->dataStart += size;
	}
}

} // namespace Program
} // namespace Monitor
