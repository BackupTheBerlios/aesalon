#include <sys/mman.h>
#include <fcntl.h>
#include <linux/futex.h>
#include <string.h>

#include <iostream>

#include "SharedMemory.h"
#include "misc/StreamAsString.h"
#include "Initializer.h"

namespace Program {

SharedMemory::SharedMemory(pid_t monitoredPid) : m_monitoredPid(monitoredPid) {
	m_shmPath = Misc::StreamAsString() << "AesalonCollector-" << monitoredPid;
	m_shmFd = shm_open(m_shmPath.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	int shmSize = Initializer::singleton()->configuration()->configItems()["shm-size"]->intValue();
	ftruncate(m_shmFd, shmSize);
	
	m_shmMemory = static_cast<uint8_t *>(mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_shmFd, 0));
	
	m_header = (MemoryMapHeader *)m_shmMemory;
	
	/* Initialize header. */
	memset(m_header, 0, shmSize);
	m_header->dataSize = shmSize;
	m_header->dataOffset = sizeof(MemoryMapHeader) + 16;
	
	/* Set up semaphores. */
	/* The two data semaphores have a default value of 0: nothing there. */
	sem_init(&m_header->dataSempahore, 1, 0);
	sem_init(&m_header->dataOverflowSemaphore, 1, 0);
	/* The two pointer semaphores have a default value of 1: they are available for locking. */
	sem_init(&m_header->dataStartSemaphore, 1, 1);
	sem_init(&m_header->dataEndSemaphore, 1, 1);
	
	m_header->dataEnd = m_header->dataStart = m_header->dataOffset;
}

SharedMemory::~SharedMemory() {
	munmap(m_shmMemory, m_header->dataSize);
	shm_unlink(m_shmPath.c_str());
}

void SharedMemory::setMainReached() {
	m_header->mainReached = 1;
}

void SharedMemory::setFinished() {
	m_header->finished = 1;
	sem_post(&m_header->dataSempahore);
}

DataPacket *SharedMemory::readPacket() {
	sem_wait(&m_header->dataSempahore);
	if(m_header->finished) {
		int left;
		sem_getvalue(&m_header->dataSempahore, &left);
		if(left == 0) return NULL;
	}
	
	/* TODO: actually read packet . . . */
	
	sem_post(&m_header->dataOverflowSemaphore);
	return NULL;
}

} // namespace Program
