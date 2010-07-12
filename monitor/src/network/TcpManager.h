#ifndef TcpNetwork_H
#define TcpNetwork_H

#include <vector>

#include "SocketManager.h"

namespace Network {

class TcpManager : public SocketManager {
public:
	TcpManager(int port);
	virtual ~TcpManager();
private:
	int m_port;
	int m_fd;
	std::vector<int> m_clientFds;
public:
	int port() const { return m_port; }
	
	virtual void sendPacket(DataPacket *packet);
	virtual void waitForConnections(int connections);
}; 

} // namespace Network

#endif
