#ifndef RendererConcreteFactory_H
#define RendererConcreteFactory_H

#include <map>

#include "Factory.h"

class ConcreteRendererFactory : public RendererFactory {
public:
	ConcreteRendererFactory();
	virtual ~ConcreteRendererFactory();
private:
	typedef std::map<std::string, Renderer *> RendererMap;
	RendererMap m_rendererMap;
public:
	virtual void registerRenderer(std::string name, Renderer *renderer);

	virtual Renderer *renderer(const std::string &name);
	virtual void rendererNames(std::vector<std::string> &names);
};

#endif
