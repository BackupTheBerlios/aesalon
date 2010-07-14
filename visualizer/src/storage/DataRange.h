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
	
	quint64 &beginTime() { return m_begin.time(); }
	const quint64 &beginTime() const { return m_begin.time(); }
	qreal &beginData() { return m_begin.data(); }
	const qreal &beginData() const { return m_begin.data(); }
	
	DataCoord &end() { return m_end; }
	const DataCoord &end() const { return m_end; }
	void setEnd(const DataCoord &end) { m_end = end; }
	
	quint64 &endTime() { return m_end.time(); }
	const quint64 &endTime() const { return m_end.time(); }
	qreal &endData() { return m_end.data(); }
	const qreal &endData() const { return m_end.data(); }
};

#endif
