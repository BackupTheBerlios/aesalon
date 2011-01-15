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
#include "common/SHMPacket.h"

namespace Monitor {
namespace Program {

ZoneReader::ZoneReader(SharedMemory *sharedMemory, Module::List *moduleList, VCommunication::DataSink *dataSink)
	: m_sharedMemory(sharedMemory), m_moduleList(moduleList), m_dataSink(dataSink) {
	
	std::cout << "moduleList: " << (void *)moduleList << std::endl;
}

ZoneReader::~ZoneReader() {
	
}

void ZoneReader::start() {
	pthread_create(&m_threadID, NULL, run, this);
}

void ZoneReader::startInThread() {
	m_threadID = pthread_self();
	run(this);
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
		
		uint32_t dataStart = 0;
		
		if(zoneHeader->head + sizeof(packetHeader) < zoneSize) {
			packetHeader = reinterpret_cast<SHMPacketHeader_t *>(zoneData + zoneHeader->head);
			dataStart = zoneHeader->head + sizeof(packetHeader);
		}
		else {
			packetHeader = reinterpret_cast<SHMPacketHeader_t *>(zoneData + ZoneDataOffset);
			dataStart = ZoneDataOffset + sizeof(packetHeader);
		}
		
		uint32_t packetSize = packetHeader->packetSize;
		uint8_t *packetData = NULL;
		if(dataStart + packetHeader->packetSize < zoneSize) {
			/* This is straightforwards, all the data is nice and contiguous. */
			packetData = zoneData + dataStart + sizeof(SHMPacketHeader_t);
			
			zoneHeader->head = dataStart + packetSize;
		}
		else {
			/* This is a bit more complex; the data is in two parts,
				thus it needs to be copied into a temporary buffer. */
			
			uint32_t underSize = zoneSize - dataStart - zoneHeader->gapSize;
			uint32_t overSize = packetSize - underSize;
			
			if(packetBufferSize < packetSize) {
				/* Replace this w/doubling behaviour? */
				packetBuffer = static_cast<uint8_t *>(realloc(packetBuffer, packetSize));
				packetBufferSize = packetSize;
			}
			memcpy(packetBuffer, zoneData + dataStart, underSize);
			memcpy(packetBuffer + underSize, zoneData + ZoneDataOffset, overSize);
			
			packetData = packetBuffer;
			
			zoneHeader->head = ZoneDataOffset + overSize;
		}
		
		std::cout << "Received packet:" << std::endl;
		std::cout << "\tsize: " << packetSize << std::endl;
		std::cout << "\tmoduleID: " << packetHeader->moduleID << std::endl;
		std::cout << "\tPID: " << zoneHeader->processID << std::endl;
		std::cout << "\tTID: " << zoneHeader->threadID << std::endl;
		
		Module::Module *module = instance->m_moduleList->module(packetHeader->moduleID);
		if(module == NULL) {
			std::cout << "Unknown packet received from module ID#" << packetHeader->moduleID << std::endl;
			continue;
		}
		Common::MarshallerInterface *interface = module->marshallerInterface();
		if(interface != NULL) {
			Common::VPacket packet(
				zoneHeader->processID,
				zoneHeader->threadID,
				packetHeader->moduleID,
				packetData,
				packetSize);
			
			interface->marhsall(instance->m_dataSink, &packet);
		}
	}
	free(packetBuffer);
	return NULL;
}

} // namespace Program
} // namespace Monitor
