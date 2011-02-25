/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/Interface.h
*/

#ifndef AesalonArtisan_Interface_H
#define AesalonArtisan_Interface_H

#include "Storage.h"
#include "Viewport.h"

namespace Artisan {

class Interface {
public:
	virtual ~Interface() {}
	
	virtual Storage *storageObject() = 0;
	
	virtual Viewport *viewport() = 0;
};

} // namespace Artisan

#endif
