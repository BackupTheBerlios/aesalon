/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/SharedMemory.h

*/

#ifndef AesalonMonitor_Program_SharedMemory_H
#define AesalonMonitor_Program_SharedMemory_H

#include <string>
#include <map>

#include "common/SharedMemoryHeader.h"
#include "common/ZoneHeader.h"

namespace Monitor {
namespace Program {

class SharedMemory {
private:
	int m_fd;
	std::string m_shmName;
	
	SharedMemoryHeader_t *m_header;
	
	uint8_t *m_zoneUseData;
	
	typedef std::map<uint32_t, uint8_t *> ZoneMap;
	ZoneMap m_zoneMap;
public:
	SharedMemory();
	~SharedMemory();
	
	uint32_t zoneCount();
	bool zoneActivated(uint32_t id);
	ZoneHeader_t *zone(uint32_t id);
private:
	void setupHeader();
	void setupConfiguration();
	void setupZoneUse();
};

} // namespace Program
} // namespace Monitor

#endif
