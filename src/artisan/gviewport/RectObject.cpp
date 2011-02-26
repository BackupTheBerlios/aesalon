/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RectObject.cpp
*/

#include <QRect>

#include "artisan/gviewport/RectObject.h"

#include "util/MessageSystem.h"

namespace Artisan {
namespace GViewport {

RectObject::RectObject(double layer, double x, double y, double w, double h) {
	m_bound.setRange(TreeType::Range(layer, layer), 0);
	m_bound.setRange(TreeType::Range(x, x+w), 1);
	m_bound.setRange(TreeType::Range(y, y+h), 2);
}

RectObject::~RectObject() {
	
}

void RectObject::render(RenderedImage &image) {
	Message(Debug, "Rendering RectObject . . .");
	double x1, y1;
	image.mapper().map(m_bound.range(1).start(), m_bound.range(2).start(), &x1, &y1);
	double x2, y2;
	image.mapper().map(m_bound.range(1).end(), m_bound.range(2).end(), &x2, &y2);
	
	Message(Debug, "Coordinates: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ")");
	
	QPainter *painter = image.painter();
	
	painter->setPen(qRgb(0, 0, 0));
	painter->setBrush(Qt::cyan);
	
	QRectF rf = QRectF(x1 * painter->device()->width(), y1 * painter->device()->height(),
		(x2-x1) * painter->device()->width(), painter->device()->height() * (y2-y1));
	
	Message(Debug, "Modified Coordinates: (" << rf.left() << "," << rf.top() << "), (" << rf.right()
		<< "," << rf.bottom() << ")");
	
	painter->drawRect(rf);
}

} // namespace GViewport
} // namespace Artisan