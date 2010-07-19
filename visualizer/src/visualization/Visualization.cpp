#include <QPointF>
#include <QPainter>
#include <QMutex>

#include "Visualization.h"
#include "VisualizationWrapper.h"

Visualization::Visualization(QSize renderSize, DataRange range) : m_range(range) {
	qDebug("renderSize: (%ix%i)", renderSize.width(), renderSize.height());
	m_image = QImage(renderSize.width(), qAbs(renderSize.height()), QImage::Format_ARGB32);
	m_image.fill(Qt::white);
	m_paintLock.unlock();
	m_wrapper = new VisualizationWrapper(this);
}

Visualization::~Visualization() {
	
}

void Visualization::merge(Visualization *other) {
	QRectF otherRect = translate(other->m_range);
	lock();
	m_painter.drawImage(otherRect.toRect(), other->m_image);
	unlock();
}

void Visualization::clear() {
	lock();
	m_image.fill(qRgb(0, 0, 0));
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
	m_paintLock.lock();
	QImage temporary = m_image;
	m_painter.begin(&m_image);
	
	m_painter.drawImage(pixels, temporary);
	
	if(pixels.x() > 0) {
		
	}
	else if(pixels.x() < 0) {
		
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
