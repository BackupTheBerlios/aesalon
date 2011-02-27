/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Rect.h
*/

#ifndef AesalonArtisan_GViewport_Rect_H
#define AesalonArtisan_GViewport_Rect_H

#include <QRect>
#include <QSize>

#include "TreeType.h"
#include "Point.h"

#include "util/StreamAsString.h"

namespace Artisan {
namespace GViewport {

class Rect {
private:
	double m_left, m_right;
	double m_top, m_bottom;
public:
	Rect() : m_left(0), m_right(0), m_top(0), m_bottom(0) {}
	Rect(double width, double height) : m_left(0), m_right(width), m_top(0), m_bottom(height) { normalize(); }
	Rect(QSizeF size) : m_left(0), m_right(size.width()), m_top(0), m_bottom(size.height()) { normalize(); }
	Rect(double left, double right, double top, double bottom)
		: m_left(left), m_right(right), m_top(top), m_bottom(bottom) { normalize(); }
	Rect(Point ul, Point lr) : 
		m_left(ul.x()), m_right(lr.x()), m_top(ul.y()), m_bottom(lr.y()) { normalize(); }
	Rect(Point ul, double width, double height)
		: m_left(ul.x()), m_right(ul.x() + width), m_top(ul.y()), m_bottom(ul.y() + height) { normalize(); }
	
	double &left() { return m_left; }
	double left() const { return m_left; }
	
	double &right() { return m_right; }
	double right() const { return m_right; }
	
	double width() const { return m_right - m_left; }
	
	double &top() { return m_top; }
	double top() const { return m_top; }
	
	double &bottom() { return m_bottom; }
	double bottom() const { return m_bottom; }
	
	double height() const { return m_bottom - m_top; }
	
	Point topLeft() const { return Point(m_left, m_top); }
	Point topRight() const { return Point(m_right, m_top); }
	Point bottomLeft() const { return Point(m_left, m_bottom); }
	Point bottomRight() const { return Point(m_right, m_bottom); }
	
	void normalize() {
		double x1 = m_left, x2 = m_right;
		double y1 = m_top, y2 = m_bottom;
		m_left = std::min(x1, x2);
		m_right = std::max(x1, x2);
		m_top = std::min(y1, y2);
		m_bottom = std::max(y1, y2);
	}
	
	TreeType::Bound toTreeBound() const {
		TreeType::Bound b;
		b.range(0) = TreeType::Range(m_left, m_right);
		b.range(1) = TreeType::Range(m_top, m_bottom);
		return b;
	}
	
	QRectF toQRect() const { 
		return QRectF(m_left, m_top, width(), height());
	}
	
	std::string toString() const {
		return Util::StreamAsString() << "(" << m_left << ", " 
			<< m_top << "), (" << m_right << ", " << m_bottom << ")";
	}
	
	Rect operator+(const Point &point) const {
		return Rect(m_left + point.x(), m_right + point.x(), m_top + point.y(), m_bottom + point.y());
	}
	
	void scaleWidth(double factor) {
		double w = width() / 2;
		double centreX = m_left + w;
		w *= factor;
		m_left = centreX - w;
		m_right = centreX + w;
	}
	
	void scaleHeight(double factor) {
		double h = height()/2;
		double centreY = m_top + h;
		h *= factor;
		m_top = centreY - h;
		m_bottom = centreY + h;
	}
};

} // namespace GViewport
} // namespace Artisan

#endif
