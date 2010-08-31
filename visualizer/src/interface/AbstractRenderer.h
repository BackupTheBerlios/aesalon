#ifndef InterfaceAbstractRenderer_H
#define InterfaceAbstractRenderer_H

#include "Renderer.h"

namespace VisualizerModule {

template <typename CacheType>
class AbstractRenderer : public Renderer {
private:
	CacheType *m_dataCache;
public:
	AbstractRenderer(CacheType *dataCache) : m_dataCache(dataCache) {}
	
	virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) = 0;
protected:
	CacheType *dataCache() const { return m_dataCache; }
};

} // namespace VisualizerModule

#endif
