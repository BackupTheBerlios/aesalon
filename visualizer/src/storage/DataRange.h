#ifndef DataRange_H
#define DataRange_H

#include "DataCoord.h"

class DataRange {
public:
	DataRange(DataCoord begin = DataCoord(), DataCoord end = DataCoord());
	~DataRange();
private:
	DataCoord m_begin, m_end;
public:
	DataCoord &begin() { return m_begin; }
	const DataCoord &begin() const { return m_begin; }
	void setBegin(const DataCoord &begin) { m_begin = begin; }
	
	uint64_t &beginTime() { return m_begin.time(); }
	const uint64_t &beginTime() const { return m_begin.time(); }
	double &beginData() { return m_begin.data(); }
	const double &beginData() const { return m_begin.data(); }
	
	DataCoord &end() { return m_end; }
	const DataCoord &end() const { return m_end; }
	void setEnd(const DataCoord &end) { m_end = end; }
	
	uint64_t &endTime() { return m_end.time(); }
	const uint64_t &endTime() const { return m_end.time(); }
	double &endData() { return m_end.data(); }
	const double &endData() const { return m_end.data(); }
};

#endif
