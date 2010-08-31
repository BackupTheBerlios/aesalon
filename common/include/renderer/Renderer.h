#ifndef RendererRenderer_H
#define RendererRenderer_H

#include "renderer/DataRange.h"
#include "renderer/VisualizationWrapper.h"

class Renderer {
public:
	virtual ~Renderer() {}
	
	virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) = 0;
};

#endif
