#ifndef DataCache_H
#define DataCache_H

#include <vector>

#include "renderer/DataCache.h"
#include "renderer/DataCoord.h"

class CpuTimeDataCache : public RendererDataCache {
public:
	virtual ~CpuTimeDataCache();
	
	typedef std::vector<DataCoord> DataVector;
	
private:
	DataVector m_dataVector;
	uint64_t m_lastCpuTime;
public:
	const DataVector &dataVector() const { return m_dataVector; }
	virtual void processPacket(DataPacket *packet);
};

#endif
