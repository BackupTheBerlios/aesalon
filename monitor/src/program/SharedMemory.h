#ifndef SharedMemory_H
#define SharedMemory_H

#include <sys/types.h>
#include <string>

#include "DataTypes.h"

namespace Program {

class SharedMemory {
public:
	SharedMemory(pid_t monitoredPid);
	virtual ~SharedMemory();
private:
	pid_t m_monitoredPid;
	std::string m_shmPath;
	int m_shmFd;
	uint8_t *m_shmMemory;
	MemoryMapHeader *m_header;
public:
	pid_t monitoredPid() const { return m_monitoredPid; }
	const std::string &shmPath() const { return m_shmPath; }
	int shmFd() const { return m_shmFd; }
	uint8_t *shmMemory() const { return m_shmMemory; }
	MemoryMapHeader *header() const { return m_header; }
	
	void setMainReached();
	void setFinished();
	
	/** Reads a packet from the shm.
		@note This function is designed to be called from the parsing thread.
		@return A received packet, or NULL if the monitored program has terminated.
	*/
	DataPacket *readPacket();
};

} // namespace Program

#endif
