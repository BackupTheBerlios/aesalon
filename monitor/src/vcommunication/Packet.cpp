/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/vcommunication/Packet.cpp

*/

#include "vcommunication/Packet.h"

namespace Monitor {
namespace VCommunication {

Packet::Packet(pid_t processID, pthread_t threadID, ModuleID moduleID, void *data, int dataSize)
	: m_processID(processID), m_threadID(threadID), m_moduleID(moduleID), m_data(data), m_dataSize(dataSize) {
	
	
}

} // namespace VCommunication
} // namespace Monitor
