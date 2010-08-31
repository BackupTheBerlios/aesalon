#ifndef DataCoord_H
#define DataCoord_H

#include "DataTypes.h"

class DataCoord {
public:
	DataCoord(uint64_t time = 0, double data = 0.0) : m_time(time), m_data(data) {}
	~DataCoord() {}
private:
	uint64_t m_time;
	double m_data;
public:
	uint64_t &time() { return m_time; }
	const uint64_t &time() const { return m_time; }
	void setTime(uint64_t time) { m_time = time; }
	
	double &data() { return m_data; }
	const double &data() const { return m_data; }
	void setData(double data) { m_data = data; }
	
	DataCoord operator+(const DataCoord &other) const {
        return DataCoord(m_time + other.m_time, m_data + other.m_data);
    }
	DataCoord operator-(const DataCoord &other) const {
        return DataCoord(m_time - other.m_time, m_data - other.m_data);
    }
};

#endif
