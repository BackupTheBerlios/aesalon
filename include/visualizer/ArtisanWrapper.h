/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/ArtisanWrapper.h
*/

#ifndef AesalonVisualizer_ArtisanWrapper_H
#define AesalonVisualizer_ArtisanWrapper_H

#include <string>

#include "artisan/Interface.h"

namespace Visualizer {

class ArtisanWrapper {
private:
	Artisan::Interface *m_interface;
	void *m_handle;
public:
	ArtisanWrapper(Artisan::Interface *interface);
	ArtisanWrapper(const std::string &name);
	~ArtisanWrapper();
	
	Artisan::Interface *interface() const { return m_interface; }
private:
	void load(const std::string &name);
};

} // namespace Visualizer

#endif
