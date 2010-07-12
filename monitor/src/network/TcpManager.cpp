#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#include "TcpManager.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"

namespace Network {

TcpManager::TcpManager(int port) : SocketManager() {
	struct addrinfo hints, *result, *rp;
	
	memset(&hints, 0, sizeof(hints));
	/* IPv4 and IPv6 are both fine. */
	hints.ai_family = AF_UNSPEC;
	/* TCP connection. */
	hints.ai_socktype = SOCK_STREAM;
	/* This is a listening socket, not a data socket. */
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	char port_str[7];
	sprintf(port_str, "%i", port);
	
	int ret = getaddrinfo(NULL, port_str, &hints, &result);
	if(ret == -1) {
		LogSystem::logNetworkMessage(Misc::StreamAsString() << "Couldn't perform DNS lookup: " << gai_strerror(ret));
		return;
	}
	
	int yes = 1;
	
	for(rp = result; rp != NULL; rp = rp->ai_next) {
		m_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(m_fd == -1) continue;
		
		if(setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) continue;
		
		if(bind(m_fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
		
		close(m_fd);
	}
	if(rp == NULL) {
		LogSystem::logNetworkMessage(Misc::StreamAsString() << "Couldn't perform port binding.");
		return;
	}
	
	if(listen(m_fd, 16) == -1) {
		LogSystem::logNetworkMessage(Misc::StreamAsString() << "Couldn't listen on port.");
		return;
	}
}

TcpManager::~TcpManager() {
	close(m_fd);
}

void TcpManager::sendPacket(DataPacket *packet) {
	for(int i = 0; i < int(m_clientFds.size()); i ++) {
		write(m_clientFds[i], &packet->dataSource, sizeof(packet->dataSource));
		write(m_clientFds[i], &packet->dataSize, sizeof(packet->dataSize));
		write(m_clientFds[i], packet->data, packet->dataSize);
	}
}

void TcpManager::waitForConnections(int connections) {
	LogSystem::logNetworkMessage(Misc::StreamAsString() << "Waiting for " << connections << " incoming connections . . .");
	for(int i = 0; i < connections; i ++) {
		m_clientFds.push_back(accept(m_fd, NULL, NULL));
	}
}

} // namespace Network
