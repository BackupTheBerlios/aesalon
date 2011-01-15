/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/SharedMemory.cpp

*/

#include <iostream>

#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "program/SharedMemory.h"
#include "common/AssertionException.h"
#include "common/Config.h"
#include "common/StreamAsString.h"
#include "common/StringTo.h"
#include "config/Vault.h"
#include "Coordinator.h"

namespace Monitor {
namespace Program {

SharedMemory::SharedMemory() {
	m_shmName = Common::StreamAsString() << "/Aesalon-" << getpid();
	
	m_fd = shm_open(m_shmName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	setupHeader();
	setupConfiguration();
	setupZones();
}

SharedMemory::~SharedMemory() {
	shm_unlink(m_shmName.c_str());
}

uint32_t SharedMemory::zoneCount() const {
	return m_header->zoneCount;
}

uint8_t *SharedMemory::zoneWithPacket() {
	for(uint32_t i = 0; i < m_header->zonesAllocated; i ++) {
		if(m_zoneUseData[i] & (0x01 << (i % 8))) {
			uint8_t *zoneData = zone(i);
			ZoneHeader_t *zoneHeader = reinterpret_cast<ZoneHeader_t *>(zoneData);

			if(sem_trywait(&zoneHeader->packetSemaphore) == -1 && errno == EAGAIN) continue;
			
			return zoneData;
		}
	}
	return NULL;
}

void SharedMemory::waitForPacket() {
	sem_wait(&m_header->packetSemaphore);
}

void SharedMemory::setupHeader() {
	ftruncate(m_fd, AesalonPageSize);
	m_header = static_cast<SharedMemoryHeader_t *>(
		mmap(NULL, AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
	
	sem_init(&m_header->packetSemaphore, 1, 0);
	sem_init(&m_header->resizeSemaphore, 1, 1);
}

uint8_t *SharedMemory::zone(uint32_t id) {
	uint8_t *data = m_zoneMap[id];
	if(data != NULL) return data;
	
	data = static_cast<uint8_t *>(mmap(NULL, AesalonPageSize*m_header->zoneSize,
		PROT_READ | PROT_WRITE, MAP_SHARED, m_fd,
		(m_header->zonePageOffset + id*m_header->zoneSize)*AesalonPageSize));
	
	m_zoneMap[id] = data;
	return data;
}

void SharedMemory::setupConfiguration() {
	char *configurationData = static_cast<char *>(
		mmap(NULL, AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, AesalonPageSize));
	m_header->configDataSize = 1;
	ftruncate(m_fd, 2*AesalonPageSize);
	
	std::vector<Config::Vault::KeyPair> configItems;
	Coordinator::instance()->vault()->match("*", configItems);
	
	uint32_t offset = 0;
	
	for(std::vector<Config::Vault::KeyPair>::iterator i = configItems.begin(); i != configItems.end(); ++i) {
		/* Ignore all internal items. */
		if(i->first.find("::") == 0) continue;
		
		while((offset + i->first.length() + i->second.length() + 2) > m_header->configDataSize*AesalonPageSize) {
			munmap(configurationData, m_header->configDataSize*AesalonPageSize);
			m_header->configDataSize ++;
			/* NOTE: the +1 is for the header. */
			ftruncate(m_fd, (m_header->configDataSize+1)*AesalonPageSize);
			configurationData = static_cast<char *>(
				mmap(NULL, m_header->configDataSize*AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED,
					m_fd, AesalonPageSize));
		}
		memcpy(&configurationData[offset], i->first.c_str(), i->first.length()+1);
		offset += i->first.length()+1;
		memcpy(&configurationData[offset], i->second.c_str(), i->second.length()+1);
		offset += i->second.length()+1;
	}
}

void SharedMemory::setupZones() {
	m_header->zoneCount = 0;
	
	m_header->zoneSize = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("zoneSize"));
	m_header->zoneUsagePages = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("zoneUsePages"));
	m_header->zonesAllocated = Common::StringTo<uint32_t>(Coordinator::instance()->vault()->get("defaultZones"));
	
	m_header->shmSize = (1 + m_header->configDataSize + m_header->zoneUsagePages +
		(m_header->zonesAllocated*m_header->zoneSize));
	
	ftruncate(m_fd, m_header->shmSize*AesalonPageSize);
}

} // namespace Program
} // namespace Monitor
