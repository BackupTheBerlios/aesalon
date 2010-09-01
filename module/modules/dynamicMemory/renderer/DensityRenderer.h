#ifndef DensityRenderer_H
#define DensityRenderer_H

#include "renderer/AbstractRenderer.h"
#include "DataCache.h"
#include "EventVisitor.h"

class DensityRenderer : public AbstractRenderer<DynamicMemoryDataCache>, public EventVisitor {
public:
	DensityRenderer(DynamicMemoryDataCache *dataCache);
	virtual ~DensityRenderer();
private:
public:
    virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort);
    virtual DataRange defaultRange() const;
    
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
};

#endif
