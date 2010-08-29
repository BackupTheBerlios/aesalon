#include <iostream>
#include <algorithm>

#include "Visualizer.h"

extern "C" {

VisualizerInterface *AesalonVisualizerCreateInstance() {
	return new CpuTimeVisualizer();
}

} // extern "C"

CpuTimeVisualizer::CpuTimeVisualizer() : m_lastElement() {
	
}

CpuTimeVisualizer::~CpuTimeVisualizer() {

}

void CpuTimeVisualizer::processIncoming(DataPacket *packet) {
	DataCoord e;
	e.setTime(packet->dataSource.timestamp);
	e.setData((double)(*(uint64_t *)packet->data));
	
	if(m_lastElement.data() != 0) {
		DataCoord adj;
		adj.setTime((e.time() + m_lastElement.time()) / 2);
		uint64_t tDiff = e.time() - m_lastElement.time();
		
		adj.setData((e.data() - m_lastElement.data()) / (double)(tDiff));
		
/*		std::cout << "\tAdjusted value: " << adj.data() << std::endl;
		std::cout << "\t\ttDiff: " << tDiff << std::endl;*/
		
		m_elementList.push_back(adj);
	}
	
	m_lastElement = e;
}

static bool compareElements(DataCoord one, DataCoord two) {
	return one.time() < two.time();
}

void CpuTimeVisualizer::visualize(VisualizationWrapper *visualization, bool *abort) {
	/*std::cout << "Asked to visualize!" << std::endl;*/
	visualization->lock();
	
	DataCoord e;
	e.setTime(visualization->range().beginTime());
	ElementList::iterator beginIterator = std::lower_bound(m_elementList.begin(), m_elementList.end(), e, compareElements);
	if(beginIterator == m_elementList.end()) {
		std::cout << "Entire visualization is out-of-bounds. Aborting." << std::endl;
		visualization->unlock();
		return;
	}
	
	e.setTime(visualization->range().endTime());
	ElementList::iterator endIterator = std::upper_bound(m_elementList.begin(), m_elementList.end(), e, compareElements);
	
	/*std::cout << "Beginning drawing loop . . ." << std::endl;*/
	
	int begin = beginIterator - m_elementList.begin();
	int end = endIterator - m_elementList.begin();
	
	/*std::cout << "\tbegin: " << begin << std::endl;
	std::cout << "\tend: " << end << std::endl;*/
	
	for(int i = begin + 1; i < end && !(*abort); i ++) {
		visualization->drawLine(m_elementList[i-1], m_elementList[i]);
	}
	
	visualization->unlock();
}

DataRange CpuTimeVisualizer::defaultDataRange() {
	std::cout << "Asked for default data range . . ." << std::endl;
	if(m_elementList.size() > 2) return DataRange(DataCoord(m_elementList.front().time(), 0.0), DataCoord(m_elementList.back().time(), 1.0));
	else return DataRange();
}
