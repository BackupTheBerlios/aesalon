#ifndef DataCache_H
#define DataCache_H

#include "renderer/DataCache.h"

class CpuTimeDataCache : public RendererDataCache {
public:
	virtual ~CpuTimeDataCache();
private:
	virtual void processPacket(DataPacket *packet);
};

#endif
