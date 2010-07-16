#ifndef Visualization_H
#define Visualization_H

#include "storage/DataRange.h"

class QPixmap;
class QPainter;
class QMutex;
class QSize;

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
	/** Checks if the visualization is locked for painting.
		@return The lock status of the visualization.
	*/
	bool isLocked() const { return m_painter.isActive(); }
	/** Re-sizes the internal QPixmap.
		@param newSize The new size of the pixmap.
	*/
	void resize(const QSize &newSize);
	
	/** Sets the pen colour. */
	void setPenColour(int r, int g, int b, int a);
	
	/** Draws a line from @a from to @a to.
		@param from The coordinates to start the line at.
		@param to The coordinates to end the line.
	*/
	void drawLine(DataCoord from, DataCoord to);
private:
	QPointF translate(const DataCoord &coord);
	QRectF translate(const DataRange &range);
};

#endif
