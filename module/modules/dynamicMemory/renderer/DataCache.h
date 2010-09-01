#ifndef DataCache_H
#define DataCache_H

#include <vector>

#include "renderer/DataCache.h"
#include "TreeHead.h"

class DynamicMemoryDataCache : public RendererDataCache {
public:
	DynamicMemoryDataCache();
	virtual ~DynamicMemoryDataCache();
private:
	typedef std::vector<TreeHead *> TreeHeadVector;
	TreeHeadVector m_treeHeadVector;
public:
    virtual void processPacket(DataPacket *packet);
private:
	void newHead();
};

#endif
