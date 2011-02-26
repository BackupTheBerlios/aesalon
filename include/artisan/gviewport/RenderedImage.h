/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/RenderedImage.h
*/

#ifndef AesalonArtisan_GViewport_RenderedImage_H
#define AesalonArtisan_GViewport_RenderedImage_H

#include <QImage>
#include <QPaintDevice>
#include <QPainter>

#include "CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

class RenderedImage {
private:
	QImage m_image;
	double m_x, m_y, m_w, m_h;
	CoordinateMapper m_mapper;
	QPainter *m_painter;
public:
	RenderedImage(int xSize, int ySize, double x, double y, double w, double h);
	RenderedImage(QImage &image, double x, double y, double w, double h);
	~RenderedImage();
	
	const CoordinateMapper &mapper() const { return m_mapper; }
	
	void startPainting();
	QPainter *painter() const { return m_painter; }
	void endPainting();
	
	void merge(const RenderedImage &other);
	
	void resize(int width, int height);
	
	void drawOnto(QPaintDevice *device);
};

} // namespace GViewport
} // namespace Artisan

#endif
