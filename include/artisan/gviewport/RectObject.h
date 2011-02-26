/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/RectObject.h
*/

#ifndef AesalonArtisan_GViewport_RectObject_H
#define AesalonArtisan_GViewport_RectObject_H

#include "Object.h"

namespace Artisan {
namespace GViewport {

class RectObject : public Object {
public:
	RectObject(double layer, double x, double y, double w, double h);
	virtual ~RectObject();
	
	virtual void render(RenderedImage &image);
};

} // namespace GViewport
} // namespace Artisan

#endif
