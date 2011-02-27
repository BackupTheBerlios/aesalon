/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Renderer.cpp
*/

#include "artisan/gviewport/Renderer.h"

#include "artisan/gviewport/Object.h"

namespace Artisan {
namespace GViewport {

Renderer::Renderer(const Rect &dataRange, const Rect &pixelRange, Data *data)
	: m_image(RenderedImage(dataRange, pixelRange)), m_data(data) {
}

Renderer::~Renderer() {

}

void Renderer::run() {
	m_data->startReading();
	
	m_data->tree().search(m_image.dataRange().toTreeBound(), this);
	
	m_data->stopReading();
	
	emit finishedRendering(m_image);
}

bool Renderer::process(const TreeType::Bound &bound, Object *value) {
	value->renderOnto(m_image);
	return true;
}

} // namespace GViewport
} // namespace Artisan
