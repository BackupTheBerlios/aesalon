#ifndef DataCache_H
#define DataCache_H

#include "visualizer/DataCache.h"

namespace VisualizerModule {

class CpuTimeDataCache : public DataCache {
public:
	CpuTimeDataCache();
	virtual ~CpuTimeDataCache();
private:
	
public:
    virtual void processPacket(DataPacket *packet);
};

} // namespace VisualizerModule

#endif
