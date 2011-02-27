/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/CoordinateMapper.h
*/

#ifndef AesalonArtisan_GViewport_CoordinateMapper_H
#define AesalonArtisan_GViewport_CoordinateMapper_H

#include "RenderedImage.h"
#include "Point.h"
#include "Rect.h"

namespace Artisan {
namespace GViewport {

class CoordinateMapper {
private:
	const Rect &m_dataRect;
	const Rect &m_pixelRect;
public:
	CoordinateMapper(const Rect &dataRect, const Rect &pixelRect);
	CoordinateMapper(const RenderedImage &m_image);
	
	Point dataToPixel(const Point &dataPoint);
	Rect dataToPixel(const Rect &dataRect);
	Point pixelToData(const Point &pixelPoint);
	Rect pixelToData(const Rect &pixelPoint);
	
	Point pixelToDataOffset(const Point &pixelPoint);
};

} // namespace GViewport
} // namespace Artisan

#endif
