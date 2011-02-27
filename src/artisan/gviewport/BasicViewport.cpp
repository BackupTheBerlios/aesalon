/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/BasicViewport.cpp
*/

#include <QResizeEvent>
#include <QPaintEvent>
#include <QThreadPool>

#include "artisan/gviewport/BasicViewport.h"
#include "artisan/gviewport/Renderer.h"

namespace Artisan {
namespace GViewport {

BasicViewport::BasicViewport(Data *data) : m_data(data) {
	setMinimumSize(120, 120);
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
}

void BasicViewport::updateRange(const Rect &range) {
	RenderedImage image(range, Rect(size()));
	image.merge(m_image);
	m_image = image;
}

void BasicViewport::resizeEvent(QResizeEvent *event) {
	RenderedImage image(m_image.dataRange(), Rect(event->size()));
	image.merge(m_image);
	m_image = image;
	Renderer *renderer = new Renderer(m_image.dataRange(), Rect(event->size()), m_data);
	connect(renderer, SIGNAL(finishedRendering(RenderedImage *)), this, SLOT(acceptRenderedImage(RenderedImage *)));
	renderer->enqueue();
}

void BasicViewport::paintEvent(QPaintEvent *event) {
	m_image.paintOnto(this);
}

} // namespace GViewport
} // namespace Artisan
