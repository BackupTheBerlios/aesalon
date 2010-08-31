#include "renderer/Controller.h"
#include "DataCache.h"
#include "GlobalUsageRenderer.h"

extern "C" {

RendererController *Instantiate() {
	RendererController *controller = new RendererController();
	RendererConcreteFactory *factory = new RendererConcreteFactory();
	CpuTimeDataCache *dataCache = new CpuTimeDataCache();
	controller->setDataCache(dataCache);

	factory->registerRenderer("Global CPU usage", new GlobalUsageRenderer(dataCache));

	return controller;
}


} // extern "C"