#include "renderer/Controller.h"
#include "DataCache.h"
#include "DensityRenderer.h"

extern "C" {

RendererController *Instantiate() {
	RendererController *controller = new RendererController();
	RendererConcreteFactory *factory = new RendererConcreteFactory();
	DynamicMemoryDataCache *dataCache = new DynamicMemoryDataCache();
	controller->setFactory(factory);
	controller->setDataCache(dataCache);
	
	factory->registerRenderer("Block density", new DensityRenderer(dataCache));
	
	return controller;
}

} // extern "C"