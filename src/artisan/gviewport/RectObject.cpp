/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RectObject.cpp
*/

#include "artisan/gviewport/RectObject.h"
#include "artisan/gviewport/CoordinateMapper.h"


namespace Artisan {
namespace GViewport {

RectObject::RectObject(const Rect &rect) : Object(rect) {

}

RectObject::~RectObject() {

}

void RectObject::renderOnto(RenderedImage &image) {
	CoordinateMapper mapper(image);
	
	image.painter().drawRect(mapper.dataToPixel(bound()).toQRect());
}

} // namespace GViewport
} // namespace Artisan