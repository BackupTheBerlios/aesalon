#ifndef DataCache_H
#define DataCache_H

#include <vector>

#include "renderer/AbstractDataCache.h"
#include "renderer/DataRange.h"
#include "TreeHead.h"
#include "CacheVisitor.h"

class DynamicMemoryDataCache : public AbstractRendererDataCache {
public:
	DynamicMemoryDataCache();
	virtual ~DynamicMemoryDataCache();
private:
	typedef std::vector<TreeHead *> TreeHeadVector;
	TreeHeadVector m_treeHeadVector;
public:
    virtual void processPacket(DataPacket *packet);
    
    void visit(const DataRange &dataRange, CacheVisitor *visitor);
    
    DataRange densityRange() const;
private:
	TreeHead *latestTree() const { return m_treeHeadVector.back(); }
	void newHead();
	void allocBlock(uint64_t address, uint64_t size, uint64_t timestamp);
	void freeBlock(uint64_t address, uint64_t timestamp);
};

#endif
