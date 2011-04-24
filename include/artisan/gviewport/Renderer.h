/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Renderer.h
*/

#ifndef AesalonArtisan_GViewport_Renderer_H
#define AesalonArtisan_GViewport_Renderer_H

#include <QRunnable>
#include <QObject>

#include "Data.h"
#include "RenderedImage.h"
#include "TreeType.h"

namespace Artisan {
namespace GViewport {

class Renderer : public QObject, public QRunnable, public TreeType::SearchVisitorType { Q_OBJECT
private:
	RenderedImage *m_image;
	Data *m_data;
public:
	Renderer(const Rect &dataRange, const Rect &pixelRange, Data *data);
	virtual ~Renderer();
	
	void enqueue();
	
	virtual void run();
	
	virtual void visit(const TreeType::BoundType &bound, Object *data);
	virtual void visit(const TreeType::PointType &point, Object *data);
signals:
	void finishedRendering(RenderedImage *image);
};

} // namespace GViewport
} // namespace Artisan

#endif
