/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Point.h
*/

#ifndef AesalonArtisan_GViewport_Point_H
#define AesalonArtisan_GViewport_Point_H

namespace Artisan {
namespace GViewport {

class Point {
private:
	double m_x, m_y;
public:
	Point();
	Point(double x, double y) : m_x(x), m_y(y) {}
	
	double &x() { return m_x; }
	double x() const { return m_x; }
	double &y() { return m_y; }
	double y() const { return m_y; }
};

} // namespace GViewport
} // namespace Artisan

#endif
