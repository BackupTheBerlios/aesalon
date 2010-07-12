#ifndef SocketManager_H
#define SocketManager_H

#include "DataTypes.h"

namespace Network {

class SocketManager {
public:
	SocketManager();
	virtual ~SocketManager();
public:
	virtual void sendPacket(DataPacket *packet) = 0;
	virtual void waitForConnections(int connections) = 0;
};

} // namespace Network

#endif
