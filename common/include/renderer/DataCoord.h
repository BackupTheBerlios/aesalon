#ifndef DataCoord_H
#define DataCoord_H

#include "DataTypes.h"

class DataCoord {
public:
	DataCoord(Timestamp time = 0, double data = 0.0) : m_time(time), m_data(data) {}
	~DataCoord() {}
private:
	Timestamp m_time;
	double m_data;
public:
	Timestamp &time() { return m_time; }
	const Timestamp &time() const { return m_time; }
	void setTime(Timestamp time) { m_time = time; }
	
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
