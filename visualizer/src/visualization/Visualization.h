#ifndef Visualization_H
#define Visualization_H

#include <QImage>
#include <QMutex>
#include <QPainter>

#include "renderer/DataRange.h"
#include "renderer/VisualizationWrapper.h"

class VisualizationController;

class Visualization : public VisualizationWrapper {
public:
	Visualization(QSize renderSize, DataRange range);
	~Visualization();
private:
	QImage m_image;
	DataRange m_range;
	QPainter m_painter;
	QMutex m_paintLock;
	VisualizationController *m_controller;
public:
	const QImage &image() const { return m_image; }
	virtual const DataRange &range() const { return m_range; }
	VisualizationController *controller() const { return m_controller; }
	void setController(VisualizationController *controller) { m_controller = controller; }
	/** Merges another visualization with this one.
		@param other The visualization to merge with.
	*/
	void merge(Visualization *other);
	
	/** Clears the visualization of all painted data. */
	virtual void clear();
	
	/** Locks the visualization for painting. */
	virtual void lock();
	/** Unlocks the visualization, signifying completion of painting. */
	virtual void unlock();
	/** Checks if the visualization is locked for painting.
		@return The lock status of the visualization.
	*/
	virtual bool isLocked() const { return m_painter.isActive(); }
	/** Re-sizes the internal QImage.
		@param newSize The new size of the image.
	*/
	void resize(const QSize &newSize);
	
	/** Sets the pen colour. */
	virtual void setPenColour(int r, int g, int b, int a);
	
	/** Draws a line from @a from to @a to.
		@param from The coordinates to start the line at.
		@param to The coordinates to end the line.
	*/
	virtual void drawLine(DataCoord from, DataCoord to);
	
	/** Creates a sub-visualization of this visualization, preserving the
		mapping ratio of data units to screen units for a given range.
		@param range The range of the sub-visualization.
		@return The new sub-visualization.
	*/
	Visualization *subVisualization(const DataRange &range);
	
	void shift(QPoint pixels);
	
	void scale(qreal zoom);
private:
	QPointF translate(const DataCoord &coord);
	QRectF translate(const DataRange &range);
	DataCoord translate(const QPoint &point);
	DataRange translate(const QRect &rect);
	
	DataCoord translateOffset(const QPoint &point);
};

#endif
