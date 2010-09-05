#include "renderer/Controller.h"
#include "DataCache.h"
#include "GlobalUsageRenderer.h"

extern "C" {

RendererController *Instantiate() {
	RendererController *controller = new RendererController();
	ConcreteRendererFactory *factory = new ConcreteRendererFactory();
	CpuTimeDataCache *dataCache = new CpuTimeDataCache();
	controller->setDataCache(dataCache);
	controller->setFactory(factory);

	factory->registerRenderer("Global CPU usage", new GlobalUsageRenderer(dataCache));

	return controller;
}


} // extern "C"