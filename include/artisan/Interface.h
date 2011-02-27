/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/Interface.h
*/

#ifndef AesalonArtisan_Interface_H
#define AesalonArtisan_Interface_H

#include "DataStore.h"
#include "Viewport.h"

namespace Artisan {

/** Represents an Aritsan module's interface to the Visualizer. */
class Interface {
public:
	virtual ~Interface() {}
	
	/** Retrieves the DataStore instance of this artisan. */
	virtual DataStore *dataStore() = 0;
	
	/** Creates a new Viewport for this artisan. */
	virtual Viewport *createViewport() = 0;
};

/** Creates the AA_Instantiate() global function in an artisan. */
#define InstantiateArtisan(type) \
	extern "C" { Artisan::Interface *AA_Instantiate() { return new type(); } }


} // namespace Artisan

#endif
