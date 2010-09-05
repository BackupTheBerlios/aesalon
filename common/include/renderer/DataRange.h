#ifndef DataRange_H
#define DataRange_H

#include "DataCoord.h"

class DataRange {
public:
	DataRange(Timestamp beginTime, double beginData, Timestamp endTime, double endData)
		: m_begin(DataCoord(beginTime, beginData)), m_end(DataCoord(endTime, endData)) {}
	DataRange(DataCoord begin = DataCoord(), DataCoord end = DataCoord()) : m_begin(begin), m_end(end) {}
	~DataRange() {}
private:
	DataCoord m_begin, m_end;
public:
	DataCoord &begin() { return m_begin; }
	const DataCoord &begin() const { return m_begin; }
	void setBegin(const DataCoord &begin) { m_begin = begin; }
	
	Timestamp &beginTime() { return m_begin.time(); }
	const Timestamp &beginTime() const { return m_begin.time(); }
	double &beginData() { return m_begin.data(); }
	const double &beginData() const { return m_begin.data(); }
	
	DataCoord &end() { return m_end; }
	const DataCoord &end() const { return m_end; }
	void setEnd(const DataCoord &end) { m_end = end; }
	
	Timestamp &endTime() { return m_end.time(); }
	const Timestamp &endTime() const { return m_end.time(); }
	double &endData() { return m_end.data(); }
	const double &endData() const { return m_end.data(); }
};

#endif
