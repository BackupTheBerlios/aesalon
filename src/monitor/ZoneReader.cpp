/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/ZoneReader.cpp
*/

#include "monitor/ZoneReader.h"
#include "shm/PacketHeader.h"
#include "util/MessageSystem.h"

namespace Monitor {

ZoneReader::ZoneReader(SHMReader *shmReader) : m_shmReader(shmReader) {

}

ZoneReader::~ZoneReader() {

}

void *ZoneReader::run(void *voidInstance) {
	ZoneReader *instance = static_cast<ZoneReader *>(voidInstance);
	SHMReader *reader = instance->m_shmReader;
	
	uint8_t *dataBuffer = new uint8_t[32];
	uint32_t dataBufferSize = 32;
	
	while(true) {
		int32_t zone = reader->zoneWithData();
		if(zone == -1) break;
		
		SHM::PacketHeader packetHeader;
		reader->readData(zone, &packetHeader, sizeof(packetHeader));
		
		if(packetHeader.packetSize > dataBufferSize) {
			while(packetHeader.packetSize > dataBufferSize) dataBufferSize *= 2;
			delete[] dataBuffer;
			dataBuffer = new uint8_t[dataBufferSize];
		}
		reader->readData(zone, dataBuffer, packetHeader.packetSize);
		
		Message(Log, "Recieved packet from module " << packetHeader.moduleID << ", size " << packetHeader.packetSize);
	}
	
	delete[] dataBuffer;
	
	return NULL;
}

} // namespace Monitor
