/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/LineObject.cpp
*/

#include "artisan/gviewport/LineObject.h"
#include "artisan/gviewport/CoordinateMapper.h"

#include "util/MessageSystem.h"

namespace Artisan {
namespace GViewport {

LineObject::LineObject(const Point &from, const Point &to) : Object(Rect(from, to)), m_from(from), m_to(to) {
	
}

LineObject::~LineObject() {
	
}

void LineObject::renderOnto(RenderedImage &image) {
	image.startPainting();
	
	CoordinateMapper mapper(image);
	
	image.painter().drawLine(
		mapper.dataToPixel(m_from).toQPoint(),
		mapper.dataToPixel(m_to).toQPoint()
		);
	
	image.stopPainting();
}

} // namespace GViewport
} // namespace Artisan