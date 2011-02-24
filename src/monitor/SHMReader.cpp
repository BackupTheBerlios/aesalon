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

SHMReader::ReadBroker::ReadBroker() : m_temporaryBuffer(NULL), m_temporaryBufferSize(0) {
	
}

SHMReader::ReadBroker::~ReadBroker() {
	if(m_temporaryBuffer) delete[] m_temporaryBuffer;
}

void SHMReader::ReadBroker::resizeBuffer(uint32_t newSize) {
	if(m_temporaryBufferSize >= newSize) return;
	delete[] m_temporaryBuffer;
	m_temporaryBuffer = new uint8_t[newSize];
}

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
			
			if(sem_trywait(&zheader->packetSemaphore) == -1 && errno == EAGAIN) continue;
			
			return i;
		}
	}
	return -1;
}

uint32_t SHMReader::zoneProcessID(uint32_t zoneID) {
	SHM::ZoneHeader *header = reinterpret_cast<SHM::ZoneHeader *>(getZone(zoneID));
	return header->processID;
}

uint32_t SHMReader::zoneThreadID(uint32_t zoneID) {
	SHM::ZoneHeader *header = reinterpret_cast<SHM::ZoneHeader *>(getZone(zoneID));
	return header->threadID;
}

void SHMReader::waitForPacket() {
	sem_wait(&m_header->packetSemaphore);
}

void SHMReader::processRequest(ReadBroker &request) {
	uint8_t *zone = getZone(request.zoneID());
	
	SHM::ZoneHeader *header = reinterpret_cast<SHM::ZoneHeader *>(zone);
	
	uint32_t start = header->head;
	uint32_t size1 = std::min(
		request.size(),
		(m_header->zoneSize*AesalonPageSize - header->gapSize) - start);
	
	uint32_t size2 = request.size() - size1;
	
	/* The difficult case; the data is in two segments. */
	if(size2 > 0) {
		request.resizeBuffer(request.size());
		memcpy(request.temporaryBuffer(), zone + start, size1);
		memcpy(request.temporaryBuffer() + size1, zone + ZoneDataOffset, size2);
		request.setData(request.temporaryBuffer());
		header->head = size2 + ZoneDataOffset;
		if(header->overflow > 0) {
			Message(Debug, "overflow > 0 in difficult case, reducing . . .");
			header->overflow -= size1;
			header->overflow -= header->gapSize;
			header->overflow -= size2;
			header->gapSize = 0;
			if(header->overflow <= 0) {
				Message(Debug, "Posting to overflow semaphore . . .");
				sem_post(&header->overflowSemaphore);
			}
		}
		else header->gapSize = 0;
	}
	/* Otherwise, the simple case. */
	else {
		request.setData(zone + start);
		header->head += size1;
		if(header->overflow > 0) {
			Message(Debug, "overflow > 0, reducing . . .");
			header->overflow -= size1;
			Message(Debug, "New overflow: " << header->overflow);
			if(header->overflow <= 0) {
				Message(Debug, "Posting to overflow semaphore . . .");
				sem_post(&header->overflowSemaphore);
			}
		}
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
		if(m_header) m_header->shmSize = start+size;
		
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
	
	m_zoneUseData = static_cast<uint8_t *>(mapRegion(m_header->configDataSize + 1, m_header->zoneUsagePages));
}

} // namespace Monitor
