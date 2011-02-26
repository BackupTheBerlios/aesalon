/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Viewport.h
*/

#ifndef AesalonArtisan_GViewport_Viewport_H
#define AesalonArtisan_GViewport_Viewport_H

#include "artisan/Viewport.h"
#include "TreeType.h"
#include "Object.h"
#include "Data.h"
#include "RenderedImage.h"
#include "Renderer.h"

namespace Artisan {
namespace GViewport {

class Viewport : public Artisan::Viewport { Q_OBJECT
private:
	Data m_data;
	RenderedImage m_rendered;
	Renderer m_renderer;
public:
	Viewport();
	virtual ~Viewport();
	
	void addObject(Object *object);
	void removeObject(Object *object);
public slots:
	void fitAll();
private slots:
	void mergeWith(RenderedImage image);
protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
};

} // namespace GViewport
} // namespace Artisan

#endif
