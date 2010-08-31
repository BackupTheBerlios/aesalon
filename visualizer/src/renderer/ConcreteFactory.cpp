#include "renderer/ConcreteFactory.h"

RendererConcreteFactory::RendererConcreteFactory() {

}

RendererConcreteFactory::~RendererConcreteFactory() {

}

void RendererConcreteFactory::registerRenderer(std::string name, Renderer *renderer) {
	this->m_rendererMap[name] = renderer;
}

Renderer *RendererConcreteFactory::renderer(const std::string &name) {
	return m_rendererMap[name];
}

void RendererConcreteFactory::rendererNames(std::vector<std::string> &names) {
	names.clear();
	for(RendererMap::iterator i = m_rendererMap.begin(); i != m_rendererMap.end(); ++i) {
		names.push_back(i->first);
	}
}
