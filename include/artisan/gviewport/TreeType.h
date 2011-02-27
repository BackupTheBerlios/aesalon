/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/TreeType.h
*/

#ifndef AesalonArtisan_GViewport_TreeType_H
#define AesalonArtisan_GViewport_TreeType_H

#include "storage/RTree.h"

namespace Artisan {
namespace GViewport {

class Object;

typedef Storage::RTree<double, Object *, 2, 64, 8> TreeType;

} // namespace GViewport
} // namespace Artisan

#endif
