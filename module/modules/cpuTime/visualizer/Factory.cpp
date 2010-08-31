#include "Factory.h"

namespace VisualizerModule {

CpuTimeFactory::CpuTimeFactory() {
	map["text"] = new ConcreteRenderer(dataCache);
}

CpuTimeFactory::~CpuTimeFactory() {
	
}

Renderer *CpuTimeFactory::createRenderer(const std::string &name) const {
	RendererMap::const_iterator i = map.find(name);
	if(found != map.end()) {
		return (*found).second;
	}
	
	....
}

void CpuTimeFactory::rendererNames(std::vector<std::string> &names) const {
	names.clear();
	for(std::size_t x = 0; x < sizeof(names) / sizeof(*names); x ++) {
		names.push_back(names[x]);
	}
}

} // namespace VisualizerModule
