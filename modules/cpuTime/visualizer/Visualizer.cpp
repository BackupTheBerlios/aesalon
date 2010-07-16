#include <iostream>
#include <algorithm>

#include "Visualizer.h"

extern "C" {

VisualizerInterface *AesalonVisualizerCreateInstance() {
	return new CpuTimeVisualizer();
}

} // extern "C"

CpuTimeVisualizer::CpuTimeVisualizer() {

}

CpuTimeVisualizer::~CpuTimeVisualizer() {

}

void CpuTimeVisualizer::processIncoming(DataPacket *packet) {
	DataElement e;
	e.timestamp = packet->dataSource.timestamp;
	e.value = *(uint64_t *)packet->data;
	if(m_elementList.size() != 0) {
		e.value -= m_elementList.back().value;
	}
	std::cout << "Received packet!" << std::endl;
	std::cout << "\tTimestamp: " << e.timestamp << std::endl;
	std::cout << "\tAdjusted value: " << e.value << std::endl;
	m_elementList.push_back(e);
}

static bool compareElements(DataElement one, DataElement two) {
	return one.timestamp < two.timestamp;
}

void CpuTimeVisualizer::visualize(VisualizationWrapper *visualization) {
	std::cout << "Asked to visualize!" << std::endl;
	visualization->lock();
	DataElement e;
	e.timestamp = visualization->dataRange().beginTime();
	ElementList::iterator begin = std::lower_bound(m_elementList.begin(), m_elementList.end(), e, compareElements);
	if(begin == m_elementList.end()) {
		std::cout << "Entire visualization is out-of-bounds. Aborting." << std::endl;
		visualization->unlock();
		return;
	}
	else if(begin == m_elementList.begin()) begin ++;
	
	e.timestamp = visualization->dataRange().endTime();
	ElementList::iterator end = std::upper_bound(m_elementList.begin(), m_elementList.end(), e, compareElements);
	
	double last_v = 0.0;
	uint64_t last_t = m_elementList.front().timestamp;
	
	std::cout << "Beginning drawing loop . . ." << std::endl;
	
	for(ElementList::iterator i = begin; i != end && i+1 != end; i ++) {
		uint64_t vDiff = i->value;
		uint64_t tDiff = i->timestamp - (i-1)->timestamp;
		
		double v = vDiff / (double)tDiff;
		uint64_t t = i->timestamp;
		
		visualization->drawLine(DataCoord(last_t, last_v), DataCoord(t, v));
		
		last_v = v;
		last_t = t;
	}
	visualization->unlock();
}

DataRange CpuTimeVisualizer::defaultDataRange() {
	std::cout << "Asked for default data range . . ." << std::endl;
	if(m_elementList.size() > 2) return DataRange(DataCoord(m_elementList.front().timestamp, 0.0), DataCoord(m_elementList.back().timestamp, 1.0));
	else return DataRange();
}
