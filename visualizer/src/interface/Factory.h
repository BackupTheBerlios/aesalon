#ifndef InterfaceFactory_H
#define InterfaceFactory_H

#include <vector>
#include <string>
#include <map>

#include "Renderer.h"

namespace VisualizerModule {

class Factory {
public:
	Factory();
	~Factory() {}
private:
	typedef std::map<std::string, Renderer *renderer> RendererMap;
	RendererMap m_rendererMap;
public:
	void registerRenderer(Renderer *renderer, std::string name);

	Renderer *renderer(const std::string &name);
	void rendererNames(std::vector<std::string> &names);
};

} // namespace VisualizerModule

#endif
