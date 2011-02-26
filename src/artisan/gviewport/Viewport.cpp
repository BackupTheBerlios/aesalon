/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Viewport.cpp
*/

#include <QPaintEvent>
#include <QResizeEvent>

#include "artisan/gviewport/Viewport.h"
#include "util/MessageSystem.h"

namespace Artisan {
namespace GViewport {

Viewport::Viewport() : m_rendered(RenderedImage(1, 1, 0.0, 0.0, 1.0, 1.0)), m_renderer(m_data) {
	setMinimumSize(120, 120);
	
	setMouseTracking(true);
	
	connect(&m_renderer, SIGNAL(renderingFinished(RenderedImage)), this, SLOT(mergeWith(RenderedImage)));
}

Viewport::~Viewport() {
	
}

void Viewport::addObject(Object *object) {
	m_data.lock();
	m_data.tree().insert(object->bound(), object);
	m_data.unlock();
}

void Viewport::removeObject(Object *object) {
	m_data.lock();
	m_data.tree().remove(object->bound(), object);
	m_data.unlock();
}

void Viewport::setDisplayRange(double x, double y, double w, double h) {
	m_rendered = RenderedImage(width(), height(), x, y, w, h);
}

void Viewport::fullRender() {
	TreeType::Bound bound;
	bound.setRange(TreeType::Range(0.0, 1.0), 0);
	bound.setRange(TreeType::Range(0.0, 1.0), 1);
	bound.setRange(TreeType::Range(0.0, 1.0), 2);
	RenderRequest request = RenderRequest(width(), height(), bound);
	
	m_renderer.render(request);
}

void Viewport::mergeWith(RenderedImage image) {
	Message(Debug, "Merging images . . .");
	m_rendered.merge(image);
	update();
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
	if(event->buttons() & Qt::LeftButton) {
		int x = event->x() - m_previous.x();
		int y = event->y() - m_previous.y();
		
		m_rendered.shiftPixels(x, y);
		
		m_previous = event->pos();
		
		Message(Debug, "m_rendered.mapper().x(): " <<  m_rendered.mapper().x());
		Message(Debug, "m_rendered.mapper().y(): " <<  m_rendered.mapper().y());
		Message(Debug, "m_rendered.mapper().w(): " <<  m_rendered.mapper().w());
		Message(Debug, "m_rendered.mapper().h(): " <<  m_rendered.mapper().h());
		
		TreeType::Bound bound;
		/* Just re-render the entire thing for the moment . . . */
		bound.setRange(TreeType::Range(0.0, 1.0), 0);
		bound.setRange(TreeType::Range(m_rendered.mapper().x(), m_rendered.mapper().x() + m_rendered.mapper().w()), 1);
		bound.setRange(TreeType::Range(m_rendered.mapper().y(), m_rendered.mapper().y() + m_rendered.mapper().h()), 2);
		
		RenderRequest request = RenderRequest(width(), height(), bound);
		
		m_renderer.render(request);
	}
}

void Viewport::mousePressEvent(QMouseEvent *event) {
	m_previous = event->pos();
}

void Viewport::mouseReleaseEvent(QMouseEvent *event) {
	
}

void Viewport::resizeEvent(QResizeEvent *event) {
	Message(Debug, "Resizing to " << event->size().width() << "x" << event->size().height());
	m_rendered.resize(event->size().width(), event->size().height());
	
	TreeType::Bound bound;
	bound.setRange(TreeType::Range(0.0, 1.0), 0);
	bound.setRange(TreeType::Range(0.0, 1.0), 1);
	bound.setRange(TreeType::Range(0.0, 1.0), 2);
	RenderRequest request = RenderRequest(event->size().width(), event->size().height(), bound);
	
	m_renderer.render(request);
}

void Viewport::paintEvent(QPaintEvent *event) {
	m_rendered.drawOnto(this);
}



} // namespace GViewport
} // namespace Artisan
