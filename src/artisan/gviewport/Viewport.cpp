/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Viewport.cpp
*/

#include "artisan/gviewport/Viewport.h"

namespace Artisan {
namespace GViewport {

Viewport::Viewport() : m_rendered(RenderedImage(1, 1, 0.0, 0.0, 1.0, 1.0)) {
	
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

} // namespace GViewport
} // namespace Artisan
