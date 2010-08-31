#include "visualizer/Controller.h"

#include "Factory.h"

namespace VisualizerModule {

extern "C" {

Controller *AesalonVisualizerCreateInstance() {
	Controller *controller = new Controller();
	controller->setFactory(new CpuTimeFactory());
	
	controller->setDataCache(NULL);
}

} // extern "C"

} // namespace VisualizerModule
