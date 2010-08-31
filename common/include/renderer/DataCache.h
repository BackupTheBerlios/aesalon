#ifndef RendererDataCache_H
#define RendererDataCache_H

#include "DataTypes.h"

/** Class to store the data used by a visualizer module. */
class RendererDataCache {
public:
	virtual ~RendererDataCache() {}
	
	virtual void processPacket(DataPacket *packet) = 0;
};

#endif
