/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/VPacket.h

*/

#ifndef AesalonCommon_VPacket_H
#define AesalonCommon_VPacket_H

#include <sys/types.h>
#include <pthread.h>

#include "ModuleID.h"

namespace Common {

class VPacket {
private:
	pid_t m_processID;
	pthread_t m_threadID;
	ModuleID m_moduleID;
	void *m_data;
	uint32_t m_dataSize;
public:
	VPacket(pid_t processID, pthread_t threadID, ModuleID moduleID, void *data, uint32_t dataSize)
		: m_processID(processID), m_threadID(threadID), m_moduleID(moduleID), m_data(data), m_dataSize(dataSize) {}
	~VPacket() {}
	
	pid_t processID() const { return m_processID; }
	pthread_t threadID() const { return m_threadID; }
	ModuleID moduleID() const { return m_moduleID; }
	
	void *data() const { return m_data; }
	uint32_t dataSize() const { return m_dataSize; }
};

} // namespace Common

#endif
