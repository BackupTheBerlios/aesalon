#ifndef RendererController_H
#define RendererController_H

#include "DataCache.h"
#include "Factory.h"
#include "ConcreteFactory.h"

class RendererController {
private:
	RendererFactory *m_factory;
	RendererDataCache *m_dataCache;
public:
	RendererFactory *factory() const { return m_factory; }
	void setFactory(RendererFactory *factory) { m_factory = factory; }
	RendererDataCache *dataCache() const { return m_dataCache; }
	void setDataCache(RendererDataCache *dataCache) { m_dataCache = dataCache; }
};

#endif
