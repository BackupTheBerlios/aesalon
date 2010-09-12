#ifndef AbstractDataCache_H
#define AbstractDataCache_H

#include "DataCache.h"

class AbstractRendererDataCache : public RendererDataCache {
private:
	uint64_t m_heartbeatTimestamp;
public:
	AbstractRendererDataCache() : m_heartbeatTimestamp(0) {}
	virtual ~AbstractRendererDataCache() {}
	
	virtual Timestamp heartbeatTimestamp() const { return m_heartbeatTimestamp; }
	virtual void updateHeartbeatTimestamp(Timestamp timestamp) { m_heartbeatTimestamp = timestamp; }
	
	virtual void processPacket(DataPacket *packet) = 0;
};

#endif
