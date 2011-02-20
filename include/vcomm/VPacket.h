/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/vcomm/VPacket.h

*/

#ifndef AesalonVComm_VPacket_H
#define AesalonVComm_VPacket_H

#include <stdint.h>

#include "ModuleID.h"

namespace VComm {

class VPacket {
private:
	uint32_t m_processID;
	uint32_t m_threadID;
	ModuleID m_moduleID;
	void *m_data;
	uint32_t m_dataSize;
public:
	VPacket(uint32_t processID, uint32_t threadID, ModuleID moduleID, void *data, uint32_t dataSize)
		: m_processID(processID), m_threadID(threadID), m_moduleID(moduleID), m_data(data), m_dataSize(dataSize) {}
	~VPacket() {}
	
	uint32_t processID() const { return m_processID; }
	uint32_t threadID() const { return m_threadID; }
	ModuleID moduleID() const { return m_moduleID; }
	
	void *data() const { return m_data; }
	uint32_t dataSize() const { return m_dataSize; }
};

} // namespace VComm

#endif
