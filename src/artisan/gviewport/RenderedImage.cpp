/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RenderedImage.cpp
*/

#include <QPaintDevice>
#include <QRect>

#include "artisan/gviewport/RenderedImage.h"
#include "artisan/gviewport/CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

RenderedImage::RenderedImage(const Rect &dataRange, const Rect &pixelSize)
	: m_dataRange(dataRange), m_pixelSize(pixelSize) {
	
	m_image = QImage(m_pixelSize.width(), m_pixelSize.height(), QImage::Format_ARGB32);
	m_painter = new QPainter();
}

RenderedImage::~RenderedImage() {
	delete m_painter;
}

void RenderedImage::merge(RenderedImage &other) {
	CoordinateMapper mapper(*this);
	Rect local = mapper.dataToPixel(other.dataRange());
	startPainting();
	
	m_painter->drawImage(local.toQRect(), other.m_image);
	
	stopPainting();
}

void RenderedImage::startPainting() {
	m_painter->begin(&m_image);
}

void RenderedImage::stopPainting() {
	m_painter->end();
}

void RenderedImage::paintOnto(QPaintDevice *device) {
	QPainter painter(device);
	painter.drawImage(QRect(0, 0, device->width(), device->height()), m_image, m_image.rect());
}

} // namespace GViewport
} // namespace Artisan
