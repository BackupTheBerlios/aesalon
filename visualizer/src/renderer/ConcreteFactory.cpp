#include "renderer/ConcreteFactory.h"

ConcreteRendererFactory::ConcreteRendererFactory() {

}

ConcreteRendererFactory::~ConcreteRendererFactory() {

}

void ConcreteRendererFactory::registerRenderer(std::string name, Renderer *renderer) {
	this->m_rendererMap[name] = renderer;
}

Renderer *ConcreteRendererFactory::renderer(const std::string &name) {
	return m_rendererMap[name];
}

void ConcreteRendererFactory::rendererNames(std::vector<std::string> &names) {
	names.clear();
	for(RendererMap::iterator i = m_rendererMap.begin(); i != m_rendererMap.end(); ++i) {
		names.push_back(i->first);
	}
}
