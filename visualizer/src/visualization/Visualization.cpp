#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QMutex>

#include "Visualization.h"

Visualization::Visualization(QSize renderSize, DataRange range) : m_range(range) {
	m_pixmap = QPixmap(renderSize);
	m_pixmap.fill();
	m_paintLock.unlock();
}

Visualization::~Visualization() {
	
}

void Visualization::merge(const Visualization &other) {
	QRectF otherRect = translate(other.m_range);
	lock();
	m_painter.drawPixmap(otherRect.toRect(), other.m_pixmap);
	unlock();
}

void Visualization::lock() {
	m_paintLock.lock();
	if(!m_painter.begin(&m_pixmap)) qWarning("Note: couldn't lock surface . . .");
}

void Visualization::unlock() {
	m_painter.end();
	m_paintLock.unlock();
}

void Visualization::resize(const QSize &newSize) {
	lock();
	QPixmap temporary = m_pixmap;
	m_pixmap = QPixmap(newSize);
	m_painter.drawPixmap(QRect(0, 0, temporary.width(), temporary.height()), temporary);
	unlock();
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
}

QPointF Visualization::translate(const DataCoord &coord) {
	quint64 xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
	
	qreal xPercentage = (coord.time() - m_range.beginTime()) / qreal(xSize);
	qreal yPercentage = (coord.data() - m_range.beginData()) / ySize;
	
	return QPointF(m_pixmap.width() * xPercentage, m_pixmap.height() * yPercentage);
}

QRectF Visualization::translate(const DataRange &range) {
	return QRectF(translate(range.begin()), translate(range.end()));
}
