#ifndef Interface_H
#define Interface_H

#include "DataTypes.h"

class VisualizerInterface {
public:
	VisualizerInterface();
	virtual ~VisualizerInterface();
public:
	virtual void processIncoming(DataPacket *packet) = 0;
};

#endif
