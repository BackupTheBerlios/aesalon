/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Object.h
*/

#ifndef AesalonArtisan_GViewport_Object_H
#define AesalonArtisan_GViewport_Object_H

#include "Rect.h"
#include "RenderedImage.h"

namespace Artisan {
namespace GViewport {

class Object {
private:
	Rect m_bound;
public:
	Object(Rect bound) : m_bound(bound) {}
	virtual ~Object() {}
	
	const Rect &bound() const { return m_bound; }
	
	virtual void renderOnto(RenderedImage &image) = 0;
};

} // namespace GViewport
} // namespace Artisan

#endif
