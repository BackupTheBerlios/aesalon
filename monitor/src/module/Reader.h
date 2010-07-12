#ifndef Reader_H
#define Reader_H

#include "DataTypes.h"
#include "ModuleMapper.h"

namespace Module {

class Reader {
public:
	Reader();
	virtual ~Reader();
private:
	ModuleMapper *m_mapper;
public:
	void processPacket(DataPacket *packet);
};

} // namespace Module

#endif
