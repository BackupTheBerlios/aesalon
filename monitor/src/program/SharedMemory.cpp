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

#include "program/SharedMemory.h"
#include "common/AssertionException.h"
#include "common/Config.h"
#include "common/StreamAsString.h"
#include "config/Vault.h"
#include "Coordinator.h"

namespace Monitor {
namespace Program {

SharedMemory::SharedMemory() {
	m_shmName = Common::StreamAsString() << "/Aesalon-" << getpid();
	
	m_fd = shm_open(m_shmName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	std::cout << "m_fd: " << m_fd << std::endl;
	std::cout << "\"" << m_shmName << "\"\n";
	
	setupHeader();
	setupConfiguration();
}

SharedMemory::~SharedMemory() {
	shm_unlink(m_shmName.c_str());
}

void SharedMemory::setupHeader() {
	ftruncate(m_fd, AesalonPageSize);
	m_header = static_cast<SharedMemoryHeader_t *>(
		mmap(NULL, AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
	
	sem_init(&m_header->packetSemaphore, 1, 0);
	sem_init(&m_header->resizeSemaphore, 1, 1);
}

void SharedMemory::setupConfiguration() {
#if 0
	/* Original code */
	std::vector<Config::Vault::KeyPair> configItems;
	Coordinator::instance()->vault()->match(*i + ":*", configItems);
	
	for(std::vector<Config::Vault::KeyPair>::iterator i = configItems.begin(); i != configItems.end(); ++i) {
		std::string envName = "AC_" + i->first;
		for(std::string::size_type s = 0; s < envName.size(); s ++) {
			if(envName[s] == ':') envName[s] = '_';
		}
		setenv(envName.c_str(), i->second.c_str(), 1);
	}
#endif
	char *configurationData = static_cast<char *>(
		mmap(NULL, AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, AesalonPageSize));
	m_header->configDataSize = 1;
	ftruncate(m_fd, 2*AesalonPageSize);
	
	std::vector<Config::Vault::KeyPair> configItems;
	Coordinator::instance()->vault()->match("*", configItems);
	
	uint32_t offset = 0;
	
	for(std::vector<Config::Vault::KeyPair>::iterator i = configItems.begin(); i != configItems.end(); ++i) {
		if(i->first.find("::") == 0) continue;
		
		/*std::cout << "name: \"" << i->first << "\"\n";
		std::cout << "data: \"" << i->second << "\"\n";*/
		/*if((offset + i->first.length() + i->second.length() + 2) > m_header->configDataSize*AesalonPageSize) {
			std::cout << "More space required!" << std::endl;
			munmap(configurationData, m_header->configDataSize*AesalonPageSize);
			m_header->configDataSize ++;
			configurationData = static_cast<char *>(
				mmap(NULL, AesalonPageSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, AesalonPageSize));
		}*/
		memcpy(&configurationData[offset], i->first.c_str(), i->first.length()+1);
		offset += i->first.length()+1;
		memcpy(&configurationData[offset], i->second.c_str(), i->second.length()+1);
		offset += i->second.length()+1;
	}
}

} // namespace Program
} // namespace Monitor
