/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/CoordinateMapper.cpp
*/

#include "artisan/gviewport/CoordinateMapper.h"

#include "util/MessageSystem.h"

namespace Artisan {
namespace GViewport {

CoordinateMapper::CoordinateMapper(double x, double y, double w, double h) : m_x(x), m_y(y), m_w(w), m_h(h) {
	
}

void CoordinateMapper::map(double fromx, double fromy, double *tox, double *toy) const {
	Message(Debug, "fromx: " << fromx);
	Message(Debug, "fromy: " << fromy);
	Message(Debug, "m_x: " << m_x);
	Message(Debug, "m_y: " << m_y);
	*tox = (fromx - 0) / m_w;
	*toy = (fromy - 0) / m_h;
}

} // namespace GViewport
} // namespace Artisan
