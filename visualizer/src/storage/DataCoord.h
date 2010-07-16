#ifndef DataCoord_H
#define DataCoord_H

#include "DataTypes.h"

class DataCoord {
public:
	DataCoord(uint64_t time = 0.0, double data = 0.0);
	~DataCoord();
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
	
	DataCoord operator+(const DataCoord &other) const;
	DataCoord operator-(const DataCoord &other) const;
};

#endif
