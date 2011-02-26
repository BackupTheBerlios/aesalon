/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/ArtisanManager.h
*/

#ifndef AesalonVisualizer_ArtisanManager_H
#define AesalonVisualizer_ArtisanManager_H

#include <string>
#include <QHash>

#include "ArtisanWrapper.h"

namespace Visualizer {

class ArtisanManager {
protected:
	typedef QHash<std::string, ArtisanWrapper *> ArtisanHash;
private:
	ArtisanHash m_artisanHash;
public:
	ArtisanManager();
	~ArtisanManager();
	
	ArtisanWrapper *artisan(const std::string &name);
};

} // namespace Visualizer

#endif
