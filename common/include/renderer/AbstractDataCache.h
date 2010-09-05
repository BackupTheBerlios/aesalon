#ifndef AbstractDataCache_H
#define AbstractDataCache_H

#include "DataCache.h"

class AbstractRendererDataCache : public RendererDataCache {
private:
	uint64_t m_heartbeatTimestamp;
public:
	virtual ~AbstractRendererDataCache() {}
	
	virtual uint64_t heartbeatTimestamp() const { return m_heartbeatTimestamp; }
	virtual void updateHeartbeatTimestamp(uint64_t timestamp) { m_heartbeatTimestamp = timestamp; }
	
	virtual void processPacket(DataPacket *packet) = 0;
};

#endif
