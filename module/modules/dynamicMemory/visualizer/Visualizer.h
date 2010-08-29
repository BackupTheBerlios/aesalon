#ifndef DynamicMemoryVisualizer_H
#define DynamicMemoryVisualizer_H

#include <vector>

#include "visualizer/Interface.h"

class DynamicMemoryVisualizer : public VisualizerInterface {
public:
	DynamicMemoryVisualizer();
	virtual ~DynamicMemoryVisualizer();
private:
	
public:
	virtual void processIncoming(DataPacket *packet);
	virtual void visualize(VisualizationWrapper *visualization, bool *abort);
	virtual DataRange defaultDataRange();
};

#endif
