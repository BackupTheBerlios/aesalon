/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/RectObject.cpp
*/

#include <QRect>

#include "artisan/gviewport/RectObject.h"

namespace Artisan {
namespace GViewport {

RectObject::RectObject(uint64_t layer, uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
	m_bound.setRange(TreeType::Range(layer, layer), 0);
	m_bound.setRange(TreeType::Range(x, x+w), 1);
	m_bound.setRange(TreeType::Range(y, y+h), 2);
}

RectObject::~RectObject() {
	
}

void RectObject::render(CoordinateMapper &mapper, QPainter &painter) {
	double x1, y1;
	mapper.map(m_bound.range(1).start(), m_bound.range(2).start(), &x1, &y1);
	double x2, y2;
	mapper.map(m_bound.range(1).end(), m_bound.range(2).end(), &x2, &y2);
	
	QRectF rf = QRectF(x1 * painter.device()->width(), y1 * painter.device()->height(),
		(x2-x1) * painter.device()->width(), painter.device()->height() * (y2-y1));
	painter.drawRect(rf);
}

} // namespace GViewport
} // namespace Artisan