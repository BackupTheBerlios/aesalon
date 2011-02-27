/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/BasicViewport.h
*/

#ifndef AesalonArtisan_GViewport_BasicViewport_H
#define AesalonArtisan_GViewport_BasicViewport_H

#include "artisan/Viewport.h"

#include "Object.h"
#include "Data.h"
#include "RenderedImage.h"

namespace Artisan {
namespace GViewport {

class BasicViewport : public Viewport { Q_OBJECT
private:
	Data *m_data;
	RenderedImage m_image;
	
	Rect m_rangeLimit;
	QPoint m_lastPoint;
public:
	BasicViewport(Data *data);
	virtual ~BasicViewport();
	
	Data *data() const { return m_data; }
	
	Rect rangeLimit() const { return m_rangeLimit; }
	void limitRange(const Rect &newLimit) { m_rangeLimit = newLimit; }
	
	void translate(const Point &upperLeft);
	void scale(double factor);
	
	void setViewport(const Rect &range);
public slots:
	void acceptRenderedImage(RenderedImage *image);
	void updateRange(const Rect &range);
protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
};

} // namespace GViewport
} // namespace Artisan

#endif
