#ifndef Visualizer_H
#define Visualizer_H

#include <vector>

#include "visualizerInterface/Interface.h"

class CpuTimeVisualizer : public VisualizerInterface {
public:
	CpuTimeVisualizer();
	virtual ~CpuTimeVisualizer();
private:
	typedef std::vector<DataCoord> ElementList;
	ElementList m_elementList;
	DataCoord m_lastElement;
public:
	virtual void processIncoming(DataPacket *packet);
	virtual void visualize(VisualizationWrapper *visualization);
	virtual DataRange defaultDataRange();
};

#endif
