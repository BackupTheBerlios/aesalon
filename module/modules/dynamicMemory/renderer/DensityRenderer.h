#ifndef DensityRenderer_H
#define DensityRenderer_H

#include "renderer/AbstractRenderer.h"
#include "DataCache.h"

class DensityRenderer : public AbstractRenderer<DynamicMemoryDataCache> {
public:
	DensityRenderer(DynamicMemoryDataCache *dataCache);
	virtual ~DensityRenderer();
private:
public:
	virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort);
	virtual DataRange defaultRange() const;
};

#endif
