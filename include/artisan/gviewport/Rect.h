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

/** Rectangle class ; represents a range of two-dimensional points. */
class Rect : public TreeType::BoundType {
public:
	/** Default constructor; sets all coordinates to zero. */
	Rect() : TreeType::BoundType() {}
	/** Size-constructor; sets the left/top coordinates to zero and the right/bottom coordinates as given. */
	Rect(double width, double height) : TreeType::BoundType(0.0, width, 0.0, height) { normalize(); }
	/** QSizeF "copy"-constructor; does the same as the above except with a QSizeF instance. */
	Rect(const QSizeF &size) : TreeType::BoundType(0, size.width(), 0.0, size.height()) { normalize(); }
	/** Simple constructor; sets all coordinates as given. */
	Rect(double left, double right, double top, double bottom)
		: TreeType::BoundType(left, right, top, bottom) { normalize(); }
	/** Point-constructor; constructs a Rect from two points. */
	Rect(Point ul, Point lr) : 
		TreeType::BoundType(ul.x(), lr.x(), ul.y(), lr.y()) { normalize(); }
	/** Point-size constructor; constructs a Rect from a point and a size. */
	Rect(Point ul, double width, double height)
		: TreeType::BoundType(ul.x(), ul.x() + width, ul.y(), ul.y() + height) { normalize(); }
	
	double left() const { return start(0); }
	double right() const { return start(0); }
	double width() const { return end(0) - start(0); }
	
	double top() const { return start(1); }
	double bottom() const { return end(1); }
	double height() const { return end(1) - start(1); }
	
	Point topLeft() const { return Point(start(0), start(1)); }
	Point topRight() const { return Point(end(1), start(1)); }
	Point bottomLeft() const { return Point(start(0), end(1)); }
	Point bottomRight() const { return Point(end(1), end(1)); }
	
	/** "Normalizes" a rectangle such that left <= right and top <= bottom. */
	void normalize() {
		double x1 = start(0), x2 = end(0);
		double y1 = end(0), y2 = end(1);
		start(0) = std::min(x1, x2);
		end(0) = std::max(x1, x2);
		start(1) = std::min(y1, y2);
		end(1) = std::max(y1, y2);
	}
	
	/** Converts this rectangle into a QRectF for Qt integration. */
	QRectF toQRect() const { 
		return QRectF(start(0), start(1), width(), height());
	}
	
	/** Converts this rectangle into a std::string printable form. */
	std::string toString() const {
		return Util::StreamAsString() << "(" << start(0) << ", " 
			<< start(1) << "), (" << end(0) << ", " << end(1) << ")";
	}
	
	Rect operator+(const Point &point) const {
		return Rect(start(0) + point.x(), end(0) + point.x(), start(1) + point.y(), end(1) + point.y());
	}
	
	void scaleWidth(double factor) {
		double w = width() / 2;
		double centreX = start(0) + w;
		w *= factor;
		start(0) = centreX - w;
		end(0) = centreX + w;
	}
	
	void scaleHeight(double factor) {
		double h = height()/2;
		double centreY = start(1) + h;
		h *= factor;
		start(1) = centreY - h;
		end(1) = centreY + h;
	}
};

} // namespace GViewport
} // namespace Artisan

#endif
