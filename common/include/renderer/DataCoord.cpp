#include "DataCoord.h"

DataCoord::DataCoord(uint64_t time, double data) : m_time(time), m_data(data) {
	
}

DataCoord::~DataCoord() {
	
}

DataCoord DataCoord::operator+(const DataCoord &other) const {
	return DataCoord(m_time + other.m_time, m_data + other.m_data);
}

DataCoord DataCoord::operator-(const DataCoord &other) const {
	return DataCoord(m_time - other.m_time, m_data - other.m_data);
}
