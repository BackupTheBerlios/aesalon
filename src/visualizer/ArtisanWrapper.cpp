/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/ArtisanWrapper.cpp
*/

#include <dlfcn.h>

#include "visualizer/ArtisanWrapper.h"
#include "util/MessageSystem.h"

namespace Visualizer {

ArtisanWrapper::ArtisanWrapper(const QString &name) {
	
}

ArtisanWrapper::~ArtisanWrapper() {
	
}

void ArtisanWrapper::load(const QString &name) {
	QString modulePath = "" + name;
	
	m_handle = dlopen(modulePath.toAscii().constData(), RTLD_NOW | RTLD_LOCAL);
	if(m_handle == NULL) {
		Message(Warning, "Could not open marshal library for module \"" << name << "\":" << dlerror());
		return;
	}
	Artisan::Interface *(*instantiate)();
	
	*(void **) (&instantiate) = dlsym(m_handle, "AM_Instantiate");
	
	if(instantiate == NULL) {
		Message(Warning, "Could not find AM_Instantiate in " << name << "'s marshal.");
		return;
	}
	
	m_interface = instantiate();
}

} // namespace Visualizer
