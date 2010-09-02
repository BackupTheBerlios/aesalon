#include <iostream>

#include "DensityRenderer.h"

DensityRenderer::DensityRenderer(DynamicMemoryDataCache *dataCache) : AbstractRenderer<DynamicMemoryDataCache>(dataCache) {
	
}

DensityRenderer::~DensityRenderer() {
	
}

void DensityRenderer::renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) {
	class Visitor : public CacheVisitor {
	public:
		bool *m_abort;
		VisualizationWrapper *m_visualization;
		virtual void handleBlock(Block *block) {
			std::cout << "Drawing box . . ." << std::endl;
			DataCoord start(block->allocTime(), double(block->address()));
			DataCoord end(block->releaseTime(), double(block->address() + block->size()));
			std::cout << "\tfrom (" << block->allocTime() << "," << block->address() << ") to (" << block->releaseTime() << "," << block->address() + block->size() << ") . . ." << std::endl;
			m_visualization->drawBox(DataRange(block->allocTime(), block->address(), block->releaseTime(), block->address() + block->size()));
    	}
	} visitor;
	visitor.m_abort = abort;
	visitor.m_visualization = visualization;
	visualization->lock();
	dataCache()->visit(range, &visitor);
	visualization->unlock();
}

DataRange DensityRenderer::defaultRange() const {
	return dataCache()->densityRange();
}
