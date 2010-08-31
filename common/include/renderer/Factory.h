#ifndef RendererFactory_H
#define RendererFactory_H

#include <vector>
#include <string>

#include "Renderer.h"

class RendererFactory {
public:
	virtual ~RendererFactory() {}
	
	virtual Renderer *renderer(const std::string &name) = 0;
	virtual void rendererNames(std::vector<std::string> &names) = 0;
};

#endif
