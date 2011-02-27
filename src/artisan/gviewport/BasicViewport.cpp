/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/BasicViewport.cpp
*/

#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>

#include "artisan/gviewport/BasicViewport.h"
#include "artisan/gviewport/Renderer.h"
#include "artisan/gviewport/CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

BasicViewport::BasicViewport(Data *data) : m_data(data) {
	setMinimumSize(120, 120);
	setMouseTracking(true);
}

BasicViewport::~BasicViewport() {
	
}

void BasicViewport::translate(const Point &upperLeft) {
	
}

void BasicViewport::scale(double factor) {
	
}

void BasicViewport::setViewport(const Rect &range) {
	RenderedImage image(range, Rect(size()));
	image.merge(m_image);
	m_image = image;
}

void BasicViewport::acceptRenderedImage(RenderedImage *image) {
	m_image.merge(*image);
	delete image;
	update();
}

void BasicViewport::updateRange(const Rect &range) {
	RenderedImage image(range, Rect(size()));
	image.merge(m_image);
	m_image = image;
	
	enqueue(new Renderer(m_image.dataRange(), m_image.pixelSize(), m_data));
}

void BasicViewport::resizeEvent(QResizeEvent *event) {
	RenderedImage image(m_image.dataRange(), Rect(event->size()));
	image.merge(m_image);
	m_image = image;
	enqueue(new Renderer(m_image.dataRange(), Rect(event->size()), m_data));
}

void BasicViewport::paintEvent(QPaintEvent *event) {
	m_image.paintOnto(this);
}

void BasicViewport::mouseMoveEvent(QMouseEvent *event) {
	if(event->buttons() & Qt::LeftButton) {
		Point diff = -(event->pos() - m_lastPoint);
		
		CoordinateMapper mapper(m_image);
		updateRange(m_image.dataRange() + mapper.pixelToDataOffset(diff));
		
		m_lastPoint = event->pos();
	}
}

void BasicViewport::mousePressEvent(QMouseEvent *event) {
	m_lastPoint = event->pos();
}

void BasicViewport::wheelEvent(QWheelEvent *event) {
	double factor = 1.0 - (event->delta() / 1200.0);
	
	Rect newRange = m_image.dataRange();
	
	if(event->modifiers() & Qt::ShiftModifier) {
		newRange.scaleHeight(factor);
	}
	else if(event->modifiers() & Qt::ControlModifier) {
		newRange.scaleWidth(factor);
	}
	else {
		newRange.scaleHeight(factor);
		newRange.scaleWidth(factor);
	}
	
	updateRange(newRange);
}

void BasicViewport::enqueue(Renderer *renderer) {
	connect(renderer, SIGNAL(finishedRendering(RenderedImage *)),
		this, SLOT(acceptRenderedImage(RenderedImage *)),
		Qt::QueuedConnection);
	renderer->enqueue();
}

} // namespace GViewport
} // namespace Artisan
