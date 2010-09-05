#include "renderer/Controller.h"
#include "DataCache.h"
#include "DensityRenderer.h"

extern "C" {

RendererController *Instantiate() {
	RendererController *controller = new RendererController();
	ConcreteRendererFactory *factory = new ConcreteRendererFactory();
	DynamicMemoryDataCache *dataCache = new DynamicMemoryDataCache();
	controller->setFactory(factory);
	controller->setDataCache(dataCache);
	
	factory->registerRenderer("Block density", new DensityRenderer(dataCache));
	
	return controller;
}

} // extern "C"
