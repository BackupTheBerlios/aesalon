/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RenderedImage.cpp
*/

#include <QRect>
#include <QPainter>

#include "artisan/gviewport/RenderedImage.h"

namespace Artisan {
namespace GViewport {

RenderedImage::RenderedImage(int xSize, int ySize, double x, double y, double w, double h)
	: m_x(x), m_y(y), m_w(w), m_h(h), m_mapper(x, y, w, h) {
	m_image = QImage(xSize, ySize, QImage::Format_ARGB32);
	m_image.fill(qRgb(255, 255, 255));
}

RenderedImage::RenderedImage(QImage &image, double x, double y, double w, double h)
	: m_image(image), m_x(x), m_y(y), m_w(w), m_h(h), m_mapper(x, y, w, h) {
	
}

RenderedImage::~RenderedImage() {
	
}

void RenderedImage::startPainting() {
	m_painter = new QPainter(&m_image);
}

void RenderedImage::endPainting() {
	delete m_painter;
	m_painter = NULL;
}

void RenderedImage::merge(const RenderedImage &other) {
	startPainting();
	
	double x, y;
	
	m_mapper.map(other.m_x, other.m_y, &x, &y);
	
	double w, h;
	
	m_mapper.map(other.m_w, other.m_h, &w, &h);
	
	Message(Debug, "Source data-rectangle: " << other.m_x << "," << other.m_y << ", size "
		<< other.m_w << "x" << other.m_h);
	Message(Debug, "Target data-rectangle: " << m_x << "," << m_y << ", size "
		<< m_w << "x" << m_h);
	Message(Debug, "Target rect: (" << x << "," << y << "), size " << w << "x" << h);
	
	m_painter->drawImage(QRectF(x*m_image.width(), y*m_image.height(), w*m_image.width(), h*m_image.height()),
		other.m_image, other.m_image.rect());
	
	endPainting();
}

void RenderedImage::resize(int width, int height) {
	QImage newImage(width, height, QImage::Format_ARGB32);
	QPainter painter(&newImage);
	
	painter.drawImage(newImage.rect(), m_image, m_image.rect());
	
	painter.end();
	
	m_image = newImage;
}

void RenderedImage::drawOnto(QPaintDevice *device) {
	QPainter painter(device);
	painter.drawImage(QRect(0, 0, device->width(), device->height()), m_image, m_image.rect());
}

} // namespace GViewport
} // namespace Artisan
