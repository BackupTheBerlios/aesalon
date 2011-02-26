/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Object.h
*/

#ifndef AesalonArtisan_GViewport_Object_H
#define AesalonArtisan_GViewport_Object_H

#include <QPainter>

#include "TreeType.h"
#include "CoordinateMapper.h"
#include "RenderedImage.h"

namespace Artisan {
namespace GViewport {

class Object {
protected:
	TreeType::Bound m_bound;
public:
	virtual ~Object() {}
	
	const TreeType::Bound &bound() const { return m_bound; }
	TreeType::Range &layerRange() { return m_bound.range(0); }
	TreeType::Range &xRange() { return m_bound.range(1); }
	TreeType::Range &yRange() { return m_bound.range(2); }
	
	virtual void render(RenderedImage &image) = 0;
};

} // namespace GViewport
} // namespace Artisan

#endif
