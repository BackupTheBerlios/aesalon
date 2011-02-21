/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/SHMReader.h
*/

#ifndef AesalonMonitor_SHMReader_H
#define AesalonMonitor_SHMReader_H

#include <stdint.h>

#include <string>
#include <vector>

#include "shm/Header.h"

namespace Monitor {

class SHMReader {
private:
	int m_fd;
	std::string m_shmName;
	
	SHM::Header *m_header;
	
	uint8_t *m_zoneUseData;
	
	typedef std::vector<uint8_t *> ZoneList;
	ZoneList m_zoneList;
public:
	SHMReader();
	~SHMReader();
	
	uint32_t zoneCount();
	int32_t zoneWithData();
	
	void waitForPacket();
	
	void readData(uint32_t zoneID, void *buffer, uint32_t size);
private:
	uint8_t *getZone(uint32_t id);
	void *mapRegion(uint32_t start, uint32_t size);
	void unmapRegion(void *data, uint32_t size);
	void setupHeader();
	void setupConfiguration();
	void setupZones();
};

} // namespace Monitor

#endif
