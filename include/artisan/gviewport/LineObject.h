/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/LineObject.h
*/

#ifndef AesalonArtisan_GViewport_LineObject_H
#define AesalonArtisan_GViewport_LineObject_H

#include "Object.h"

namespace Artisan {
namespace GViewport {

class LineObject : public Object {
public:
	LineObject(double layer, double x1, double y1, double x2, double y2);
	virtual ~LineObject();
	
	virtual void render(RenderedImage &image);
};

} // namespace GViewport
} // namespace Artisan

#endif
