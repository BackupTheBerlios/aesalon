/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/CoordinateMapper.cpp
*/

#include "artisan/gviewport/CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

CoordinateMapper::CoordinateMapper(double x, double y, double w, double h) : m_x(x), m_y(y), m_w(w), m_h(h) {
	
}

void CoordinateMapper::map(uint64_t fromx, uint64_t fromy, double *tox, double *toy) {
	*tox = fromx - m_x;
	*toy = fromy - m_y;
	
	*tox /= m_w;
	*toy /= m_h;
}

} // namespace GViewport
} // namespace Artisan
