#ifndef Visualizer_H
#define Visualizer_H

#include <vector>

#include "visualizerInterface/Interface.h"

class DataElement {
public:
	uint64_t timestamp;
	uint64_t value;
};

class CpuTimeVisualizer : public VisualizerInterface {
public:
	CpuTimeVisualizer();
	virtual ~CpuTimeVisualizer();
private:
	typedef std::vector<DataElement> ElementList;
	ElementList m_elementList;
	uint64_t m_lastElementValue;
public:
	virtual void processIncoming(DataPacket *packet);
	virtual void visualize(VisualizationWrapper *visualization);
	virtual DataRange defaultDataRange();
};

#endif
