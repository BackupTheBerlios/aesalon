#include <iostream>
#include "DataCache.h"

CpuTimeDataCache::~CpuTimeDataCache() {

}

void CpuTimeDataCache::processPacket(DataPacket *packet) {
	uint64_t cpuTime = *(uint64_t *)packet->data;
	if(m_dataVector.size() == 0) {
		m_dataVector.push_back(DataCoord(packet->dataSource.timestamp, 0.0));
	}
	else {
		uint64_t ddiff = cpuTime - m_lastCpuTime;
		Timestamp tdiff = packet->dataSource.timestamp - m_dataVector.back().time();
		
		double usage = ddiff / (double)tdiff;
		
		std::cout << "Reading in " << packet->dataSource.timestamp << ", usage: " << usage << std::endl;
		std::cout << "\tddiff: " << ddiff << std::endl;
		std::cout << "\ttdiff: " << tdiff << std::endl;
		
		m_dataVector.push_back(DataCoord(packet->dataSource.timestamp, usage));
	}
	m_lastCpuTime = cpuTime;
}
