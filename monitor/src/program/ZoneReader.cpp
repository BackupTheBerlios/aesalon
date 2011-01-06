/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/ZoneReader.cpp

*/

#include <stdlib.h>
#include <string.h>

#include "program/ZoneReader.h"
#include "Coordinator.h"
#include "common/Config.h"
#include "common/StringTo.h"

namespace Monitor {
namespace Program {

ZoneReader::ZoneReader(SharedMemory *sharedMemory, Module::List *moduleList) :
	m_sharedMemory(sharedMemory), m_moduleList(moduleList) {
	
}

ZoneReader::~ZoneReader() {
	
}

void ZoneReader::start() {
	m_threadID = pthread_self();
	run(this);
}

void ZoneReader::startInThread() {
	pthread_create(&m_threadID, NULL, run, this);
}

void ZoneReader::join() {
	pthread_join(m_threadID, NULL);
}

void *ZoneReader::run(void *voidInstance) {
	uint32_t zoneSize = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("zoneSize"))*AesalonPageSize;
	uint8_t *packetBuffer = NULL;
	uint32_t packetBufferSize = 0;
	
	ZoneReader *instance = static_cast<ZoneReader *>(voidInstance);
	while(true) {
		instance->m_sharedMemory->waitForPacket();
		uint8_t *zoneData = instance->m_sharedMemory->zoneWithPacket();
		if(zoneData == NULL) continue;
		
		ZoneHeader_t *zoneHeader = reinterpret_cast<ZoneHeader_t *>(zoneData);
		SHMPacketHeader_t *packetHeader = NULL;
		/* TODO: handle case where packetHeader is not exactly where it should be, e.g. has wrapped. */
		packetHeader = reinterpret_cast<SHMPacketHeader_t *>(zoneData + zoneHeader->head);
		
		uint32_t packetSize = packetHeader->packetSize;
		uint8_t *packetData = NULL;
		if(zoneHeader->head + packetHeader->packetSize < zoneSize) {
			/* This is straightforwards, all the data is nice and contiguous. */
			packetData = zoneData + zoneHeader->head + sizeof(SHMPacketHeader_t);
			
			zoneHeader->head += packetSize + sizeof(SHMPacketHeader_t);
		}
		else {
			/* This is a bit more complex; the data is in two parts,
				thus it needs to be copied into a temporary buffer. */
			
			uint32_t underSize = zoneSize - (zoneHeader->head + sizeof(packetHeader)) - zoneHeader->gapSize;
			uint32_t overSize = packetSize - underSize;
			
			if(packetBufferSize < packetSize) {
				packetBuffer = static_cast<uint8_t *>(realloc(packetBuffer, packetSize));
				packetBufferSize = packetSize;
			}
			memcpy(packetBuffer, zoneData + zoneHeader->head, underSize);
			memcpy(packetBuffer + underSize, zoneData + ZoneDataOffset, overSize);
			
			packetData = packetBuffer;
			
			zoneHeader->head = ZoneDataOffset + overSize;
		}
		/* TODO: process packet. Data is in packetData, size of packet is packetSize, and header is packetHeader. */
		/* Other associated information such as the PID and TID of the source are in zoneHeader. */
	}
	free(packetBuffer);
	return NULL;
}

} // namespace Program
} // namespace Monitor
