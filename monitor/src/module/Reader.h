#ifndef Reader_H
#define Reader_H

#include "DataTypes.h"
#include "ModuleMapper.h"
#include "network/SocketManager.h"
#include "program/Logger.h"

namespace Module {

class Reader {
public:
	Reader();
	virtual ~Reader();
private:
	ModuleMapper *m_mapper;
	Network::SocketManager *m_socketManager;
	Program::Logger *m_logger;
public:
	void processPacket(DataPacket *packet);
};

} // namespace Module

#endif
