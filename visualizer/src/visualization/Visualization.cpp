#include <QPointF>
#include <QPainter>
#include <QMutex>

#include "Visualization.h"
#include "VisualizationController.h"

Visualization::Visualization(QSize renderSize, DataRange range) : m_range(range) {
	/*qDebug("renderSize: (%ix%i)", renderSize.width(), renderSize.height());*/
	if(renderSize.width() == 0) renderSize.setWidth(1);
	if(renderSize.height() == 0) renderSize.setHeight(1);
	m_image = QImage(qAbs(renderSize.width()), qAbs(renderSize.height()), QImage::Format_ARGB32);
	m_image.fill(Qt::white);
}

Visualization::~Visualization() {
	
}

void Visualization::merge(Visualization *other) {
	lock();
	other->lock();
	QRectF otherRect = translate(other->m_range).normalized();
	
	qDebug("merge: this size is (%i,%i) . . .", m_image.width(), m_image.height());
	qDebug("merge: otherRect is (%f,%f),(%f,%f) . . .", otherRect.left(), otherRect.top(), otherRect.right(), otherRect.bottom());
	
	m_painter.setBrush(Qt::white);
	m_painter.setPen(Qt::NoPen);
	m_painter.drawRect(otherRect);
	m_painter.drawImage(otherRect.toRect(), other->m_image);
	
	other->unlock();
	unlock();
}

void Visualization::clear() {
	lock();
	m_image.fill(qRgb(255, 255, 255));
	unlock();
}

void Visualization::lock() {
	m_paintLock.lock();
	/*qDebug("Locked surface . . .");*/
	if(!m_painter.begin(&m_image)) {
		qWarning("Note: couldn't lock surface . . .");
	}
}

void Visualization::unlock() {
	/*qDebug("Unlocking surface . . .");*/
	m_painter.end();
	m_paintLock.unlock();
}

void Visualization::resize(const QSize &newSize) {
	/*qDebug("Resizing . . .");*/
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

void Visualization::setFillColour(int r, int g, int b, int a) {
	QBrush brush = m_painter.brush();
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(qRgba(r, g, b, a));
	m_painter.setBrush(brush);
}

void Visualization::drawLine(DataCoord from, DataCoord to) {
	if(!isLocked()) {
		qWarning("drawLine() called when visualization is not locked.");
		return;
	}
	QLineF line(translate(from), translate(to));
	/*qDebug("Asked to draw line from (%f, %f) to (%f, %f) . . .", line.x1(), line.y1(), line.x2(), line.y2());*/
	
	m_painter.drawLine(line);
	m_controller->modifiedPoint(from);
	m_controller->modifiedPoint(to);
}

void Visualization::drawBox(DataRange range) {
	if(!isLocked()) {
		qWarning("drawBox() called when visualization is not locked.");
		return;
	}
	QRectF rect(translate(range).normalized());
	
	if(rect.left() < 0) rect.setLeft(0.0);
	else if(rect.right() > m_image.width()) rect.setRight(m_image.width());
	
	if(rect.top() < 0) rect.setTop(0.0);
	else if(rect.bottom() > m_image.height()) rect.setBottom(m_image.height());
	
	qDebug("Drawing box from (%f, %f) to (%f, %f) . . .", rect.left(), rect.top(), rect.right(), rect.bottom());
	
	m_painter.drawRect(rect);
	m_controller->modifiedPoint(range.begin());
	m_controller->modifiedPoint(range.end());
}

void Visualization::touch(Timestamp timestamp) {
	m_controller->modifiedTime(timestamp);
}

Visualization *Visualization::subVisualization(const DataRange &range) {
	Visualization *sv = new Visualization(translate(range).toAlignedRect().size(), range);
	sv->setController(m_controller);
	return sv;
}

void Visualization::shift(DataCoord by) {
	m_paintLock.lock();
	
	qDebug("\n\tbeginTime(): %li\n", m_controller->totalRange().beginTime());
	if(by.time() < 0 && m_range.beginTime() + by.time() < m_controller->totalRange().beginTime()) by.time() = 0.0;
	if(by.time() > 0 && m_range.endTime() + by.time() > m_controller->totalRange().endTime()) by.time() = 0.0;
	
	if(by.data() < 0 && m_range.beginData() + by.data() < m_controller->totalRange().beginData()) by.data() = 0.0;
	if(by.data() > 0 && m_range.endData() + by.data() > m_controller->totalRange().endData()) by.data() = 0.0;
	
	QPointF pixels = translateOffset(by);
	qDebug("by: %li, %f", by.time(), by.data());
	qDebug("pixels: %f,%f", pixels.x(), pixels.y());
	QImage temporary = m_image.copy();
	m_painter.begin(&m_image);
	
	m_image.fill(qRgb(255, 192, 192));
	
	m_painter.drawImage(pixels, temporary);
	
	/*m_painter.setBrush(Qt::white);
	m_painter.setPen(Qt::NoPen);*/
	
	m_range.begin() += by;
	m_range.end() += by;
	
	m_painter.end();
	m_paintLock.unlock();
}

void Visualization::scale(qreal x, qreal y) {
	m_paintLock.lock();
	
	Timestamp xSize = m_range.endTime() - m_range.beginTime();
	double ySize = m_range.endData() - m_range.beginData();
	
	Timestamp xCentre = (m_range.endTime() + m_range.beginTime()) / 2;
	double yCentre = (m_range.endData() + m_range.beginData()) / 2.0;
	
	DataCoord newBegin = DataCoord((Timestamp)(xCentre - ((xSize / 2) * x)), yCentre - ((ySize / 2) * y));
	DataCoord newEnd = DataCoord((Timestamp)(xCentre + ((xSize / 2) * x)), yCentre + ((ySize / 2) * y));
	
	QImage temporary = m_image;
	m_painter.begin(&m_image);
	
	m_image.fill(qRgb(255, 255, 255));
	QRectF rect = translate(DataRange(newBegin, newEnd)).normalized();
	/*qDebug("rect: (%f,%f),(%f,%f)", rect.left(), rect.top(), rect.right(), rect.bottom());*/
	m_painter.drawImage(translate(DataRange(
		DataCoord((Timestamp)(xCentre - ((xSize / 2) * 1/x)), yCentre - ((ySize / 2) * 1/y)),
		DataCoord((Timestamp)(xCentre + ((xSize / 2) * 1/x)), yCentre + ((ySize / 2) * 1/y))
		)).normalized(), temporary);
	
	m_painter.end();
	
	m_range.setBegin(newBegin);
	m_range.setEnd(newEnd);
	
	m_paintLock.unlock();
	
	m_controller->fullVisualization();
}

QPointF Visualization::translate(const DataCoord &coord) {
	Timestamp xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
		
	qreal xPercentage = ((qreal)coord.time() - (qreal)m_range.beginTime()) / qreal(xSize);
	qreal yPercentage = (coord.data() - m_range.beginData()) / ySize;
	
	return QPointF((m_image.width()-1) * xPercentage, (m_image.height()-1) - ((m_image.height()-1) * yPercentage));
}

QRectF Visualization::translate(const DataRange &range) {
	return QRectF(translate(range.begin()), translate(range.end()));
}

DataCoord Visualization::translate(const QPoint &point) {
	qreal xPercentage = point.x() / (qreal)(m_image.width()-1);
	qreal yPercentage = point.y() / (qreal)(m_image.height()-1);
	
	Timestamp xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
	
	return DataCoord((xPercentage * xSize) + m_range.beginTime(), (yPercentage * ySize) + m_range.beginData());
}

DataRange Visualization::translate(const QRect &rect) {
	return DataRange(translate(rect.topLeft()), translate(rect.bottomRight()));
}

DataCoord Visualization::translateOffset(const QPoint &point) {
	qreal xPercentage = point.x() / (qreal)(m_image.width()-1);
	qreal yPercentage = point.y() / (qreal)(m_image.height()-1);
	
	Timestamp xSize = m_range.endTime() - m_range.beginTime();
	qreal ySize = m_range.endData() - m_range.beginData();
	
	return DataCoord((xPercentage * xSize), (yPercentage * ySize));
}

QPointF Visualization::translateOffset(const DataCoord &coord) {
	DataCoord c = coord;
	c.time() += m_range.beginTime();
	c.data() += m_range.endData();
	QPointF translated = translate(c);
	translated.setX(-translated.x());
	translated.setY(-translated.y());	
	return translated;
}
