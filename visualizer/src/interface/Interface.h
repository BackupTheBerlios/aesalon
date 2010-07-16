#ifndef Interface_H
#define Interface_H

#include "DataTypes.h"
#include "visualization/Visualization.h"

class VisualizerInterface {
public:
	VisualizerInterface();
	virtual ~VisualizerInterface();
public:
	virtual void processIncoming(DataPacket *packet) = 0;
	
	virtual void visualize(Visualization *visualization) = 0;
};

#endif
