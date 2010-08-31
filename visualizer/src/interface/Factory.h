#ifndef Factory_H
#define Factory_H

#include <vector>
#include <string>

#include "Renderer.h"

namespace VisualizerModule {

class Factory {
public:
	virtual ~Factory() {}

	virtual Renderer *createRenderer(const std::string &name) const = 0;
	
	virtual void rendererNames(std::vector<std::string> &names) const = 0;
};

} // namespace VisualizerModule

#endif
