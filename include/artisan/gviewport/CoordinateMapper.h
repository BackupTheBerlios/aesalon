/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/CoordinateMapper.h
*/

#ifndef AesalonArtisan_GViewport_CoordinateMapper_H
#define AesalonArtisan_GViewport_CoordinateMapper_H

#include "TreeType.h"

namespace Artisan {
namespace GViewport {

class CoordinateMapper {
private:
	double m_x, m_y, m_w, m_h;
public:
	CoordinateMapper(double x, double y, double w, double h);
	~CoordinateMapper() {}
	
	void map(uint64_t fromx, uint64_t fromy, double *tox, double *toy);
};

} // namespace GViewport
} // namespace Artisan

#endif
