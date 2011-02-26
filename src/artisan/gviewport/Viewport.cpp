/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Viewport.cpp
*/

#include "artisan/gviewport/Viewport.h"

namespace Artisan {
namespace GViewport {

Viewport::Viewport() {
	
}

Viewport::~Viewport() {
	
}

void Viewport::addObject(Object *object) {
	m_tree.insert(object->bound(), object);
}

} // namespace GViewport
} // namespace Artisan
