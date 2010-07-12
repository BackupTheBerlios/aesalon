#ifndef Reader_H
#define Reader_H

#include "DataTypes.h"
#include "ModuleMapper.h"
#include "network/SocketManager.h"

namespace Module {

class Reader {
public:
	Reader();
	virtual ~Reader();
private:
	ModuleMapper *m_mapper;
	Network::SocketManager *m_socketManager;
public:
	void processPacket(DataPacket *packet);
};

} // namespace Module

#endif
