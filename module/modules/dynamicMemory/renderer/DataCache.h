#ifndef DataCache_H
#define DataCache_H

#include <vector>

#include "renderer/DataCache.h"
#include "renderer/DataRange.h"
#include "TreeHead.h"
#include "EventVisitor.h"

class DynamicMemoryDataCache : public RendererDataCache {
public:
	DynamicMemoryDataCache();
	virtual ~DynamicMemoryDataCache();
private:
	typedef std::vector<TreeHead *> TreeHeadVector;
	TreeHeadVector m_treeHeadVector;
public:
    virtual void processPacket(DataPacket *packet);
    
    void visit(EventVisitor *visitor, const DataRange &range);
private:
	void newHead();
};

#endif
