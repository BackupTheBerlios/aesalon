/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RenderedImage.cpp
*/

#include "artisan/gviewport/RenderedImage.h"

namespace Artisan {
namespace GViewport {

RenderedImage::RenderedImage(int xSize, int ySize, double x, double y, double w, double h)
	: m_x(x), m_y(y), m_w(w), m_h(h) {
	m_image = QImage(xSize, ySize, QImage::Format_ARGB32);
}

RenderedImage::RenderedImage(QImage &image, double x, double y, double w, double h)
	: m_image(image), m_x(x), m_y(y), m_w(w), m_h(h) {
	
}

RenderedImage::~RenderedImage() {
	
}

} // namespace GViewport
} // namespace Artisan
