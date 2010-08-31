#ifndef InterfaceDataCache_H
#define InterfaceDataCache_H

#include "DataTypes.h"

namespace VisualizerModule {

/** Class to store the data used by a visualizer module. */
class DataCache {
public:
	virtual ~DataCache() {}
	
	virtual void processPacket(DataPacket *packet) = 0;
};

} // namespace VisualizerModule

#endif
