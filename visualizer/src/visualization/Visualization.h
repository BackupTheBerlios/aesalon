#ifndef Visualization_H
#define Visualization_H

#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QMutex>

#include "storage/DataRange.h"

class Visualization {
public:
	Visualization(QSize renderSize, DataRange range);
	~Visualization();
private:
	QPixmap m_pixmap;
	DataRange m_range;
	QPainter m_painter;
	QMutex m_paintLock;
public:
	const QPixmap &pixmap() const { return m_pixmap; }
	/** Merges another visualization with this one.
		@param other The visualization to merge with.
	*/
	void merge(const Visualization &other);
	
	/** Locks the visualization for painting. */
	void lock();
	/** Unlocks the visualization, signifying completion of painting. */
	void unlock();
	
	void resize(const QSize &newSize);
private:
	QPointF translate(const DataCoord &coord);
	QRectF translate(const DataRange &range);
};

#endif
