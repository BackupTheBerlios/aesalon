/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Renderer.h
*/

#ifndef AesalonArtisan_GViewport_Renderer_H
#define AesalonArtisan_GViewport_Renderer_H

#include <QThread>

#include "Data.h"
#include "RenderedImage.h"
#include "CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

class Renderer : public QThread { Q_OBJECT
private:
	Data &m_data;
public:
	Renderer(Data &data);
	~Renderer();
public slots:
	void render(TreeType::Bound bound, CoordinateMapper mapper) {}
signals:
	void renderingFinished(RenderedImage image);
};

} // namespace GViewport
} // namespace Artisan

#endif
