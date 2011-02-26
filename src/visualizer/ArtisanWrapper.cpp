/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/ArtisanWrapper.cpp
*/

#include <dlfcn.h>

#include "visualizer/ArtisanWrapper.h"
#include "config/GlobalVault.h"
#include "util/MessageSystem.h"

namespace Visualizer {

ArtisanWrapper::ArtisanWrapper(Artisan::Interface *interface) : m_interface(interface) {
	
}

ArtisanWrapper::ArtisanWrapper(const std::string &name) {
	load(name);
}

ArtisanWrapper::~ArtisanWrapper() {
	
}

void ArtisanWrapper::load(const std::string &name) {
	std::string modulePath = Config::GlobalVault::instance()->get(name + ":root") + name;
	
	m_handle = dlopen(modulePath.c_str(), RTLD_NOW | RTLD_LOCAL);
	if(m_handle == NULL) {
		Message(Warning, "Could not open artisan library for module \"" << name << "\":" << dlerror());
		return;
	}
	Artisan::Interface *(*instantiate)();
	
	*(void **) (&instantiate) = dlsym(m_handle, "AA_Instantiate");
	
	if(instantiate == NULL) {
		Message(Warning, "Could not find AA_Instantiate in " << name << "'s artisan.");
		return;
	}
	
	m_interface = instantiate();
}

} // namespace Visualizer
