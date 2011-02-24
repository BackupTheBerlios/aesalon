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

void ZoneReader::run() {
	run(this);
}

void ZoneReader::start() {
	pthread_create(&m_threadID, NULL, run, this);
}

void ZoneReader::wait() {
	pthread_join(m_threadID, NULL);
}

void *ZoneReader::run(void *voidInstance) {
	Message(Debug, "Beginning ZoneReader loop . . .");
	ZoneReader *instance = static_cast<ZoneReader *>(voidInstance);
	SHMReader *reader = instance->m_shmReader;
	SHMReader::ReadBroker broker;
	
	while(true) {
		reader->waitForPacket();
		int32_t zone = reader->zoneWithData();
		if(zone == -1) break;
		
		SHM::PacketHeader *packetHeader;
		broker.setupRequest(zone, sizeof(SHM::PacketHeader));
		reader->processRequest(broker);
		packetHeader = static_cast<SHM::PacketHeader *>(broker.data());
		
		uint8_t *packetData;
		broker.setupRequest(zone, packetHeader->packetSize);
		reader->processRequest(broker);
		packetData = static_cast<uint8_t *>(broker.data());
		
		Message(Log, "Recieved packet from module " << packetHeader->moduleID << ", size " << packetHeader->packetSize);
		
		
	}
	
	Message(Debug, "Ending ZoneReader loop . . .");
	
	return NULL;
}

} // namespace Monitor
