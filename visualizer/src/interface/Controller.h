#ifndef InterfaceController_H
#define InterfaceController_H

#include "DataCache.h"
#include "Factory.h"

namespace VisualizerModule {

class Controller {
private:
	Factory *m_factory;
	DataCache *m_dataCache;
public:
	Factory *factory() const { return m_factory; }
	void setFactory(Factory *factory) { m_factory = factory; }
	DataCache *dataCache() const { return m_dataCache; }
	void setDataCache(DataCache *dataCache) { m_dataCache = dataCache; }
};

} // namespace VisualizerModule

#endif
