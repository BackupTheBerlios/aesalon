/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Viewport.h
*/

#ifndef AesalonArtisan_GViewport_Viewport_H
#define AesalonArtisan_GViewport_Viewport_H

#include "artisan/Viewport.h"
#include "TreeType.h"
#include "Object.h"

namespace Artisan {
namespace GViewport {

class Viewport : public Artisan::Viewport {
private:
	TreeType m_tree;
public:
	Viewport();
	virtual ~Viewport();
	
	void addObject(Object *object);
};

} // namespace GViewport
} // namespace Artisan

#endif
