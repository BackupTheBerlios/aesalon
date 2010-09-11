#ifndef RendererAbstractRenderer_H
#define RendererAbstractRenderer_H

#include "Renderer.h"

template <typename CacheType>
class AbstractRenderer : public Renderer {
private:
	CacheType *m_dataCache;
public:
	AbstractRenderer(CacheType *dataCache) : m_dataCache(dataCache) {}
	
	virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) = 0;
	virtual DataRange defaultRange() const = 0;
protected:
	CacheType *dataCache() const { return m_dataCache; }
};

#endif