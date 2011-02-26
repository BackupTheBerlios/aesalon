/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/RenderedImage.h
*/

#ifndef AesalonArtisan_GViewport_RenderedImage_H
#define AesalonArtisan_GViewport_RenderedImage_H

#include <QImage>

namespace Artisan {
namespace GViewport {

class RenderedImage {
private:
	QImage m_image;
	double m_x, m_y, m_w, m_h;
public:
	RenderedImage(int xSize, int ySize, double x, double y, double w, double h);
	RenderedImage(QImage &image, double x, double y, double w, double h);
	~RenderedImage();
};

} // namespace GViewport
} // namespace Artisan

#endif
