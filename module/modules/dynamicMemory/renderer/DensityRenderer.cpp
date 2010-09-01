#include "DensityRenderer.h"

DensityRenderer::DensityRenderer(DynamicMemoryDataCache *dataCache) : AbstractRenderer<DynamicMemoryDataCache>(dataCache) {
	
}

DensityRenderer::~DensityRenderer() {
	
}

void DensityRenderer::renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) {
	if(*abort) return;
	dataCache()->visit(this, range);
}

DataRange DensityRenderer::defaultRange() const {
	return DataRange();
}

void DensityRenderer::visit(AllocEvent *event) {

}

void DensityRenderer::visit(FreeEvent *event) {
	
}
