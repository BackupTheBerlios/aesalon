/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/RenderRequest.h
*/

#ifndef AesalonArtisan_GViewport_RenderRequest_H
#define AesalonArtisan_GViewport_RenderRequest_H

#include "CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

class RenderRequest {
private:
	int m_width, m_height;
	TreeType::Bound m_bound;
public:
	RenderRequest(int width, int height, const TreeType::Bound &bound)
		: m_width(width), m_height(height), m_bound(bound) {}
	
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	
	const TreeType::Bound &bound() const { return m_bound; }
};

} // namespace GViewport
} // namespace Artisan

#endif
