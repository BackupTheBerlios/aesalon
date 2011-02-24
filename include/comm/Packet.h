/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/comm/Packet.h
*/

#ifndef AesalonComm_Packet_H
#define AesalonComm_Packet_H

#include <stdint.h>
#include "ModuleID.h"

namespace Comm {

class Packet {
private:
	ModuleID m_moduleID;
	uint32_t m_processID;
	uint32_t m_threadID;
	uint32_t m_dataSize;
	uint8_t *m_data;
public:
	Packet(ModuleID moduleID, uint32_t processID, uint32_t threadID, uint32_t dataSize, uint8_t *data)
		: m_moduleID(moduleID), m_processID(processID), m_threadID(threadID), m_dataSize(dataSize),
		m_data(data) {}
	
	ModuleID moduleID() const { return m_moduleID; }
	uint32_t processID() const { return m_processID; }
	uint32_t threadID() const { return m_threadID; }
	uint32_t dataSize() const { return m_dataSize; }
	uint8_t *data() const { return m_data; }
};

} // namespace Comm

#endif
