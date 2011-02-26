/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/LineObject.cpp
*/

#include <QRect>


#include "artisan/gviewport/LineObject.h"

namespace Artisan {
namespace GViewport {

LineObject::LineObject(double layer, double x1, double y1, double x2, double y2) {
	m_bound.setRange(TreeType::Range(layer, layer), 0);
	m_bound.setRange(TreeType::Range(x1, x2), 1);
	m_bound.setRange(TreeType::Range(y1, y2), 2);
}

LineObject::~LineObject() {

}

void LineObject::render(RenderedImage &image) {
	Message(Debug, "Rendering LineObject . . .");
	double x1, y1;
	image.mapper().map(m_bound.range(1).start(), m_bound.range(2).start(), &x1, &y1);
	double x2, y2;
	image.mapper().map(m_bound.range(1).end(), m_bound.range(2).end(), &x2, &y2);
	
	Message(Debug, "Coordinates: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ")");
	
	QPainter *painter = image.painter();
	
	painter->setPen(qRgb(0, 0, 0));
	
	painter->drawLine(x1 * painter->device()->width(), y1 * painter->device()->height(),
		x2 * painter->device()->width(), y2 * painter->device()->height());
}

} // namespace GViewport
} // namespace Artisan