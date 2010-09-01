#ifndef GlobalUsageRenderer_H
#define GlobalUsageRenderer_H

#include "renderer/AbstractRenderer.h"
#include "DataCache.h"

class GlobalUsageRenderer : public AbstractRenderer<CpuTimeDataCache> {
public:
    GlobalUsageRenderer(CpuTimeDataCache *dataCache);
    virtual ~GlobalUsageRenderer();
private:
public:
    virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort);
    virtual DataRange defaultRange() const;
};

#endif
