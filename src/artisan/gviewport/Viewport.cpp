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

void Viewport::fitAll() {

}

void Viewport::mergeWith(RenderedImage image) {
	Message(Debug, "Merging images . . .");
	m_rendered.merge(image);
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
