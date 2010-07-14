#ifndef Interface_H
#define Interface_H

#include "DataTypes.h"

class ModuleInterface {
public:
	ModuleInterface();
	virtual ~ModuleInterface();
public:
	virtual void processIncoming(DataPacket *packet) = 0;
};

#endif
