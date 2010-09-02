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
			m_visualization->drawBox(DataRange(block->allocTime(), block->address(), block->releaseTime(), block->address() + block->size()));
    	}
	} visitor;
	visitor.m_abort = abort;
	visitor.m_visualization = visualization;
	dataCache()->visit(range, &visitor);
}

DataRange DensityRenderer::defaultRange() const {
	return DataRange();
}
