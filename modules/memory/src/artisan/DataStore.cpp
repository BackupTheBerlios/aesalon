#include "DataStore.h"

DataStore::DataStore() {
	m_firstTime = 0.0;
}

DataStore::~DataStore() {

}

void DataStore::process(Comm::Packet *packet) {
	uint64_t timestamp = reinterpret_cast<uint64_t *>(packet->data())[0];
	uint64_t cpuTime = reinterpret_cast<uint64_t *>(packet->data())[1];
	
	if(m_firstTime == 0.0) {
		m_firstTime = timestamp;
	}
	else {
		double value = double(cpuTime-m_lastCpuTime) / (timestamp - m_lastTime);
		m_dataList.push_back(QPair<double, double>(timestamp - m_firstTime, value));
	}
	
	m_lastTime = timestamp;
	m_lastCpuTime = cpuTime;
}
