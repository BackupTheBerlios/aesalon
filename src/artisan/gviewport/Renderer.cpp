/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Renderer.cpp
*/

#include <QThreadPool>

#include "artisan/gviewport/Renderer.h"
#include "artisan/gviewport/Object.h"

#include "util/MessageSystem.h"

namespace Artisan {
namespace GViewport {

Renderer::Renderer(const Rect &dataRange, const Rect &pixelRange, Data *data) : m_data(data) {
	m_image = new RenderedImage(dataRange, pixelRange);
}

Renderer::~Renderer() {
	
}

void Renderer::enqueue() {
	QThreadPool::globalInstance()->start(this);
}

void Renderer::run() {
	m_data->startReading();
	m_image->startPainting();
	
	m_data->tree().search(m_image->dataRange(), *this);
	
	m_image->stopPainting();
	m_data->stopReading();
	
	emit finishedRendering(m_image);
}

void Renderer::visit(const TreeType::BoundType &bound, Object *data) {
	data->renderOnto(*m_image);
}

void Renderer::visit(const TreeType::PointType &point, Object *data) {
	data->renderOnto(*m_image);
}

} // namespace GViewport
} // namespace Artisan
