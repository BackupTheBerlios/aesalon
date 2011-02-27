/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/RenderedImage.h
*/

#ifndef AesalonArtisan_GViewport_RenderedImage_H
#define AesalonArtisan_GViewport_RenderedImage_H

#include <QPainter>
#include <QImage>

#include "Rect.h"

namespace Artisan {
namespace GViewport {

class RenderedImage {
private:
	Rect m_dataRange;
	Rect m_pixelSize;
	QImage m_image;
	QPainter *m_painter;
public:
	RenderedImage(const Rect &dataRange = Rect(), const Rect &pixelSize = Rect());
	~RenderedImage();
	
	const Rect &dataRange() const { return m_dataRange; }
	const Rect &pixelSize() const { return m_pixelSize; }
	
	void merge(RenderedImage &other);
	
	void startPainting();
	QPainter &painter() { return *m_painter; }
	void stopPainting();
	
	void paintOnto(QPaintDevice *device);
};

} // namespace GViewport
} // namespace Artisan

#endif
