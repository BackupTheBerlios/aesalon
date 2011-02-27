/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/BasicViewport.cpp
*/

#include "artisan/gviewport/BasicViewport.h"

namespace Artisan {
namespace GViewport {

BasicViewport::BasicViewport(Data *data) : m_data(data) {
	
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

void BasicViewport::acceptRenderedImage(RenderedImage image) {
	m_image.merge(image);
}

void BasicViewport::updateRange(const Rect &range) {
	
}

} // namespace GViewport
} // namespace Artisan