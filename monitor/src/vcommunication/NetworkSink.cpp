/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/vcommunication/NetworkSink.cpp

*/

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#include "vcommunication/NetworkSink.h"
#include "common/StringTo.h"
#include "common/StringToBool.h"
#include "Coordinator.h"

#include "common/Config.h"

namespace Monitor {
namespace VCommunication {

NetworkSink::NetworkSink() {
	openSocket();
	
	sem_init(&m_accessLock, 0, 1);
}

NetworkSink::~NetworkSink() {
	sem_destroy(&m_accessLock);

	closeSocket();
}

void NetworkSink::sinkPacket(Common::VPacket *packet) {
	if(m_serverFd == -1) return;
	
	sem_wait(&m_accessLock);
	
	for(ClientFdList::iterator i = m_clientFds.begin(); i != m_clientFds.end(); ++i) {
		sendPacket(packet, *i);
	}
	
	sem_post(&m_accessLock);
}

void NetworkSink::openSocket() {
	m_serverFd = -1;
	std::string port = Coordinator::instance()->vault()->get("tcpPort");
	if(port == "") {
		return;
	}
	struct addrinfo hints;
	
	memset(&hints, 0, sizeof(hints));
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	int rv = 0;
	struct addrinfo *result;
	if((rv = getaddrinfo(NULL, port.c_str(), &hints, &result)) == -1) {
		std::cout << gai_strerror(rv) << std::endl;
		return;
	}
	
	int yes = 1;
	
	struct addrinfo *rp;
	for(rp = result; rp != NULL; rp = rp->ai_next) {
		m_serverFd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(m_serverFd == -1) continue;
		
		if(setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) continue;
		
		if(bind(m_serverFd, rp->ai_addr, rp->ai_addrlen) == 0) break;
		
		close(m_serverFd);
	}
	if(rp == NULL) {
		m_serverFd = -1;
		return;
	}
	
	freeaddrinfo(result);
	
	if(listen(m_serverFd, AesalonConnectionQueueLength) == -1) {
		m_serverFd = -1;
		return;
	}
	
	int connectionCount = Common::StringTo<int>(Coordinator::instance()->vault()->get("networkWaitCount"));
	bool shouldWait = Common::StringToBool(Coordinator::instance()->vault()->get("networkWait"));
	if(shouldWait) waitForConnections(connectionCount);
}

void NetworkSink::closeSocket() {
	if(m_serverFd != -1) {
		close(m_serverFd);
	}
}

void NetworkSink::waitForConnections(int connectionCount) {
	for(int i = 0; i < connectionCount; i ++) {
		int fd = accept(m_serverFd, NULL, NULL);
		if(fd == -1) {
			i --;
			continue;
		}
		m_clientFds.push_back(fd);
	}
}

void NetworkSink::sendPacket(Common::VPacket *packet, int fd) {
	ModuleID moduleID = packet->moduleID();
	write(fd, &moduleID, sizeof(moduleID));
	
	pid_t processID = packet->processID();
	write(fd, &processID, sizeof(processID));
	
	pthread_t threadID = packet->threadID();
	write(fd, &threadID, sizeof(threadID));
	
	uint32_t dataSize = packet->dataSize();
	write(fd, &dataSize, sizeof(dataSize));
	
	write(fd, packet->data(), dataSize);
}

} // namespace VCommunication
} // namespace Monitor
