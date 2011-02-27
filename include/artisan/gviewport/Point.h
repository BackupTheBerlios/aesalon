/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Point.h
*/

#ifndef AesalonArtisan_GViewport_Point_H
#define AesalonArtisan_GViewport_Point_H

#include <QPoint>

namespace Artisan {
namespace GViewport {

/** A single point of data in two-dimensional space. */
class Point {
private:
	/** The x-coordinate of this point. */
	double m_x;
	/** The y-coordinate of this point. */
	double m_y;
public:
	/** Default constructor; sets m_x = m_y = 0. */
	Point() : m_x(0), m_y(0) {}
	/** Simple constructor; sets the x/y coordinates as given. */
	Point(double x, double y) : m_x(x), m_y(y) {}
	/** QPoint "copy"-constructor; initializes this point to the value of a QPoint instance. */
	Point(const QPoint &point) : m_x(point.x()), m_y(point.y()) {}
	/** QPointF "copy"-constructor; initializes this point to the value of a QPointF instance. */
	Point(const QPointF &point) : m_x(point.x()), m_y(point.y()) {}
	
	/** Returns the x coordinate as a non-const reference for reading or modifying. */
	double &x() { return m_x; }
	/** Returns the x coordinate as a value reference for reading. */
	double x() const { return m_x; }
	/** Returns the y coordinate as a non-const reference for reading or modifying. */
	double &y() { return m_y; }
	/** Returns the y coordinate as a value reference for reading. */
	double y() const { return m_y; }
	
	/** Addition operator; adds the x/y coordinates of this point and another together and returns the result as a
		new point.
		@param other The second point.
		@return The resulting point.
	*/
	Point operator+(const Point &other) const {
		return Point(m_x + other.m_x, m_y + other.m_y);
	}
	
	/** Converts this point to a QPointF instance. Useful for Qt integration. */
	QPointF toQPoint() const {
		return QPointF(m_x, m_y);
	}
};

} // namespace GViewport
} // namespace Artisan

#endif
