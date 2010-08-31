#ifndef Renderer_H
#define Renderer_H

#include "storage/DataRange.h"
#include "visualization/VisualizationWrapper.h"

namespace VisualizerModule {

class Renderer {
public:
	virtual ~Renderer() {}
	
	virtual void renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) = 0;
};

} // namespace VisualizerModule

#endif
