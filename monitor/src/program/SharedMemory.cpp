#include <sys/mman.h>
#include <fcntl.h>

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
}

SharedMemory::~SharedMemory() {
	munmap(m_shmMemory, m_header->dataSize);
	shm_unlink(m_shmPath.c_str());
}

} // namespace Program
