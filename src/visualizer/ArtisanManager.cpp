/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/ArtisanManager.cpp
*/

#include "visualizer/StringHacks.h"
#include "visualizer/ArtisanManager.h"


namespace Visualizer {

ArtisanManager::ArtisanManager() {

}

ArtisanManager::~ArtisanManager() {

}

ArtisanWrapper *ArtisanManager::artisan(const std::string &name) {
	ArtisanWrapper *artisan = m_artisanHash[name];
	if(artisan != NULL) return artisan;
	
	artisan = new ArtisanWrapper(name);
	if(artisan->interface() == NULL) {
		delete artisan;
		return NULL;
	}
	
	m_artisanHash[name] = artisan;
	emit newArtisan(name);
	
	return artisan;
}

QList<std::string> ArtisanManager::artisanList() {
	return m_artisanHash.keys();
}

} // namespace Visualizer
