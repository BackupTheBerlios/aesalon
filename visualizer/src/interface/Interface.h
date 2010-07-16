#ifndef Interface_H
#define Interface_H

#include "DataTypes.h"
#include "storage/DataRange.h"
#include "visualization/VisualizationWrapper.h"

class VisualizerInterface {
public:
	VisualizerInterface();
	virtual ~VisualizerInterface();
public:
	virtual void processIncoming(DataPacket *packet) = 0;
	
	virtual void visualize(VisualizationWrapper *visualization) = 0;
	virtual DataRange defaultDataRange() = 0;
};

#endif
