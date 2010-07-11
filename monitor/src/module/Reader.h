#ifndef Reader_H
#define Reader_H

#include "DataTypes.h"

namespace Module {

class Reader {
public:
	Reader();
	virtual ~Reader();
public:
	void processPacket(DataPacket *packet);
};

} // namespace Module

#endif
