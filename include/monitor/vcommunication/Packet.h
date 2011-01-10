/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/vcommunication/Packet.h

*/

#ifndef AesalonMonitor_VCommunication_Packet_H
#define AesalonMonitor_VCommunication_Packet_H

#include <sys/types.h>
#include <pthread.h>

#include "common/ModuleID.h"

namespace Monitor {
namespace VCommunication {

class Packet {
private:
	pid_t m_processID;
	pthread_t m_threadID;
	ModuleID m_moduleID;
	void *m_data;
	int m_dataSize;
public:
	Packet(pid_t processID, pthread_t threadID, ModuleID moduleID, void *data, int dataSize);
	~Packet() {}
	
	pid_t processID() const { return m_processID; }
	pthread_t threadID() const { return m_threadID; }
	ModuleID moduleID() const { return m_moduleID; }
	
	void *data() const { return m_data; }
	int dataSize() const { return m_dataSize; }
};

} // namespace VCommunication
} // namespace Monitor

#endif
