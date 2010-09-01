#include "renderer/Controller.h"
#include "DataCache.h"

extern "C" {

RendererController *Instantiate() {
	RendererController *controller = new RendererController();
	RendererConcreteFactory *factory = new RendererConcreteFactory();
	DynamicMemoryDataCache *dataCache = new DynamicMemoryDataCache();
	controller->setFactory(factory);
	controller->setDataCache(dataCache);
	
	
	
	return controller;
}

} // extern "C"