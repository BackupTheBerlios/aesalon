/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/DataStore.h
*/

#ifndef AesalonArtisan_DataStore_H
#define AesalonArtisan_DataStore_H

#include "comm/Packet.h"

namespace Artisan {

class DataStore {
public:
	virtual ~DataStore() {}
	
	virtual void process(Comm::Packet *packet) = 0;
};

} // namespace Artisan

#endif
