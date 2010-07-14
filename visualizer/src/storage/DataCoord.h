#ifndef DataCoord_H
#define DataCoord_H

#include <QtGlobal>

class DataCoord {
public:
	DataCoord(quint64 time = 0.0, qreal data = 0.0);
	~DataCoord();
private:
	quint64 m_time;
	qreal m_data;
public:
	quint64 &time() { return m_time; }
	const quint64 &time() const { return m_time; }
	void setTime(quint64 time) { m_time = time; }
	
	qreal &data() { return m_data; }
	const qreal &data() const { return m_data; }
	void setData(qreal data) { m_data = data; }
	
	DataCoord operator+(const DataCoord &other) const;
	DataCoord operator-(const DataCoord &other) const;
};

#endif
