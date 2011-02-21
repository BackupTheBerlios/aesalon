/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/SHMReader.cpp
*/

#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>

#include "Config.h"

#include "monitor/SHMReader.h"
#include "monitor/Coordinator.h"
#include "util/StreamAsString.h"
#include "util/MessageSystem.h"
#include "util/StringTo.h"
#include "shm/ZoneHeader.h"

namespace Monitor {

SHMReader::SHMReader() : m_header(NULL) {
	m_shmName = Util::StreamAsString() << "/Aesalon-" << getpid();
	
	m_fd = shm_open(m_shmName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(m_fd == -1) Message(Fatal, "Could not open shared memory: " << strerror(errno));
	
	setupHeader();
	setupConfiguration();
	setupZones();
}

SHMReader::~SHMReader() {
	
}

uint32_t SHMReader::zoneCount() {
	return m_header->zoneCount;
}

int32_t SHMReader::zoneWithData() {
	for(uint32_t i = 0; i < m_header->zonesAllocated; i ++) {
		if(m_zoneUseData[i % 8] & (0x01 << (i % 8))) {
			uint8_t *zone = getZone(i);
			
			if(zone == NULL) {
				Message(Warning, "Could not open zone #" << i);
				continue;
			}
			
			SHM::ZoneHeader *zheader = reinterpret_cast<SHM::ZoneHeader *>(zone);
			
			if(sem_wait(&zheader->packetSemaphore) == -1 && errno == EAGAIN) continue;
			
			return i;
		}
	}
	return -1;
}

void SHMReader::waitForPacket() {
	sem_wait(&m_header->packetSemaphore);
}

void SHMReader::readData(uint32_t zoneID, void *buffer, uint32_t size) {
	uint8_t *zone = getZone(zoneID);
	
	SHM::ZoneHeader *header = reinterpret_cast<SHM::ZoneHeader *>(zone);
	
	/* Wait for the data to become available . . . */
	
	
	/* Copy the data. */
	uint32_t start1 = header->head + ZoneDataOffset;
	uint32_t size1 = std::min(size, (m_header->zoneSize*AesalonPageSize)-start1);
	
	uint32_t size2 = size - size1;
	uint32_t start2 = ZoneDataOffset;
	
	memcpy(buffer, zone + start1, size1);
	if(size2 > 0) {
		memcpy(static_cast<uint8_t *>(buffer) + size1, zone + start2, size2);
	}
}

uint8_t *SHMReader::getZone(uint32_t id) {
	if(id < m_zoneList.size() && m_zoneList[id] != NULL) {
		return m_zoneList[id];
	}
	
	uint8_t *zone =
		static_cast<uint8_t *>(mapRegion(m_header->zonePageOffset + id*m_header->zoneSize, m_header->zoneSize));
	
	m_zoneList.resize(id+1);
	m_zoneList[id] = zone;
	
	return zone;
}

void *SHMReader::mapRegion(uint32_t start, uint32_t size) {
	if(m_header == NULL || m_header->shmSize < (start+size)) {
		if(m_header) sem_wait(&m_header->resizeSemaphore);
		
		Message(Debug, "Resizing SHM to " << start+size << " page(s).");
		if(ftruncate(m_fd, (start+size) * AesalonPageSize) != 0) {
			Message(Fatal, "Could not resize shared memory.");
		}
		
		if(m_header) sem_post(&m_header->resizeSemaphore);
	}
	
	void *memory = 
		mmap(NULL, size * AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, start * AesalonPageSize);
	
	return memory;
}

void SHMReader::unmapRegion(void *data, uint32_t size) {
	munmap(data, size*AesalonPageSize);
}

void SHMReader::setupHeader() {
	m_header = reinterpret_cast<SHM::Header *>(mapRegion(0, 1));
	
	sem_init(&m_header->packetSemaphore, 1, 0);
	sem_init(&m_header->resizeSemaphore, 1, 1);
}

void SHMReader::setupConfiguration() {
	char *configurationData = static_cast<char *>(mapRegion(1, 1));
	m_header->configDataSize = 1;
	
	std::vector<Config::Vault::KeyPair> configItems;
	Coordinator::instance()->vault()->match("*", configItems);
	
	uint32_t offset = 0;
	
	for(std::vector<Config::Vault::KeyPair>::iterator i = configItems.begin(); i != configItems.end(); ++i) {
		/* Ignore all internal items. */
		if(i->first.find("::") == 0) continue;
		
		while((offset + i->first.length() + i->second.length() + 2) > m_header->configDataSize*AesalonPageSize) {
			unmapRegion(configurationData, m_header->configDataSize);
			m_header->configDataSize ++;
			configurationData = static_cast<char *>(mapRegion(1, m_header->configDataSize));
		}
		memcpy(&configurationData[offset], i->first.c_str(), i->first.length()+1);
		offset += i->first.length()+1;
		memcpy(&configurationData[offset], i->second.c_str(), i->second.length()+1);
		offset += i->second.length()+1;
	}
}

void SHMReader::setupZones() {
	m_header->zoneCount = 0;
	m_header->zoneSize = Util::StringTo<uint32_t>(Coordinator::instance()->vault()->get("zoneSize"));
	m_header->zoneUsagePages = Util::StringTo<uint32_t>(Coordinator::instance()->vault()->get("zoneUsePages"));
	
	m_header->zonePageOffset = m_header->configDataSize + 1 + m_header->zoneUsagePages;
}

} // namespace Monitor
