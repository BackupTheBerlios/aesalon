#ifndef DataCache_H
#define DataCache_H

#include <vector>

#include "renderer/DataCache.h"
#include "renderer/DataRange.h"
#include "TreeHead.h"
#include "CacheVisitor.h"

class DynamicMemoryDataCache : public RendererDataCache {
public:
	DynamicMemoryDataCache();
	virtual ~DynamicMemoryDataCache();
private:
	typedef std::vector<TreeHead *> TreeHeadVector;
	TreeHeadVector m_treeHeadVector;
public:
    virtual void processPacket(DataPacket *packet);
    
    void visit(const DataRange &dataRange, CacheVisitor *visitor);
private:
	void visit(TreeHead *tree, uint64_t start, uint64_t end);
	TreeHead *latestTree() const { return m_treeHeadVector.back(); }
	void newHead();
	void allocBlock(uint64_t address, uint64_t size, uint64_t timestamp);
	void freeBlock(uint64_t address, uint64_t timestamp);
};

#endif
