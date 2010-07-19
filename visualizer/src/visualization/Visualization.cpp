#include <QPointF>
#include <QPainter>
#include <QMutex>

#include "Visualization.h"
#include "VisualizationWrapper.h"
#include "VisualizationController.h"

Visualization::Visualization(QSize renderSize, DataRange range) : m_range(range) {
	qDebug("renderSize: (%ix%i)", renderSize.width(), renderSize.height());
	m_image = QImage(qAbs(renderSize.width()), qAbs(renderSize.height()), QImage::Format_ARGB32);
	m_image.fill(Qt::white);
	m_wrapper = new VisualizationWrapper(this);
}

Visualization::~Visualization() {
	
}

void Visualization::merge(Visualization *other) {
	QRectF otherRect = translate(other->m_range);
	lock();
	m_painter.drawImage(otherRect.normalized().toRect(), other->m_image);
	unlock();
}

void Visualization::clear() {
	lock();
	m_image.fill(qRgb(255, 255, 255));
	unlock();
}

void Visualization::lock() {
	qDebug("Locking surface . . .");
	m_paintLock.lock();
	if(!m_painter.begin(&m_image)) {
		qWarning("Note: couldn't lock surface . . .");
	}
}

void Visualization::unlock() {
	qDebug("Unlocking surface . . .");
	m_painter.end();
	m_paintLock.unlock();
}

void Visualization::resize(const QSize &newSize) {
	qDebug("Resizing . . .");
	m_paintLock.lock();
	QImage temporary = m_image;
	m_image = QImage(newSize, QImage::Format_ARGB32);
	m_painter.begin(&m_image);
	m_painter.drawImage(QRect(0, 0, temporary.width(), temporary.height()), temporary);
	m_painter.end();
	m_paintLock.unlock();
}

void Visualization::setPenColour(int r, int g, int b, int a) {
	QPen pen = m_painter.pen();
	pen.setColor(qRgba(r, g, b, a));
	m_painter.setPen(pen);
}

void Visualization::drawLine(DataCoord from, DataCoord to) {
	if(!isLocked()) {
		qWarning("drawLine() called when visualization is not locked.");
		return;
	}
	QLineF line(translate(from), translate(to));
	qDebug("Asked to draw line from (%f, %f) to (%f, %f) . . .", line.x1(), line.y1(), line.x2(), line.y2());
	
	m_painter.drawLine(line);
}

Visualization *Visualization::subVisualization(const DataRange &range) {
	return new Visualization(translate(range).size().toSize(), range);
}

void Visualization::shift(QPoint pixels) {
	qDebug("Asked to shift visualization . . .");
	m_paintLock.lock();
	QImage temporary = m_image;
	m_painter.begin(&m_image);
	
	m_painter.drawImage(pixels, temporary);
	
	m_painter.setBrush(Qt::white);
	m_painter.setPen(Qt::NoPen);
	
	if(pixels.x() > 0) {
		QRect rect = QRect(0, 0, pixels.x(), m_image.height());
		m_painter.drawRect(rect);
		m_controller->renderRegion(translate(rect));
	}
	else if(pixels.x() < 0) {
		QRect rect = QRect(m_image.width() - pixels.x(), 0, pixels.x(), m_image.height());
		m_painter.drawRect(rect);
		m_controller->renderRegion(translate(rect));
	}
	
	if(pixels.y() > 0) {
		m_painter.drawRect(0, m_image.height() - pixels.y(), m_image.width(), pixels.y());
	}
	else if(pixels.y() < 0) {
		m_painter.drawRect(0, 0, m_image.width(), pixels.y());
	}
	
	m_painter.end();
	m_paintLock.unlock();
}

QPointF Visualization::translate(const DataCoord &coord) {
	quint64 xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
	
	qreal xPercentage = (coord.time() - m_range.beginTime()) / qreal(xSize);
	qreal yPercentage = (coord.data() - m_range.beginData()) / ySize;
	
	return QPointF(m_image.width() * xPercentage, m_image.height() - (m_image.height() * yPercentage));
}

QRectF Visualization::translate(const DataRange &range) {
	return QRectF(translate(range.begin()), translate(range.end()));
}

DataCoord Visualization::translate(const QPoint &point) {
	qreal xPercentage = point.x() / (qreal)m_image.width();
	qreal yPercentage = point.y() / (qreal)m_image.height();
	
	quint64 xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
	
	return DataCoord((xPercentage * xSize) + m_range.beginTime(), (yPercentage * ySize) + m_range.beginData());
}

DataRange Visualization::translate(const QRect &rect) {
	return DataRange(translate(rect.topLeft()), translate(rect.bottomRight()));
}
