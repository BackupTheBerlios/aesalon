/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/CompositeDataStore.h
*/

#ifndef AesalonArtisan_CompositeDataStore_H
#define AesalonArtisan_CompositeDataStore_H

#include "DataStore.h"

namespace Artisan {

class CompositeDataStore {
public:
	virtual ~CompositeDataStore() {}
	
	virtual void addDataStore(DataStore *dataStore) = 0;
	virtual void removeDataStore(DataStore *dataStore) = 0;
	
	virtual void useDataStore(DataStore *dataStore, bool use=true) = 0;
};

} // namespace Artisan

#endif
