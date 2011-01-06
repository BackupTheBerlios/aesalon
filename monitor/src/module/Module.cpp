/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/module/Module.cpp

*/

#include <iostream>

#include <dlfcn.h>

#include "module/Module.h"
#include "monitor/Coordinator.h"

namespace Monitor {
namespace Module {

Module::Module(const std::string &moduleName) : m_loaded(false) {
	std::string path =
		Coordinator::instance()->vault()->get(moduleName + ":root")
		+ Coordinator::instance()->vault()->get(moduleName + ":polisherPath");
	
	m_moduleHandle = dlopen(path.c_str(), RTLD_LOCAL | RTLD_NOW);
	m_instance = NULL;
	if(m_moduleHandle != NULL) {
		Common::PolisherInterface *(*instantiate)() = NULL;
		
		*(void **)(&instantiate) = dlsym(m_moduleHandle, "AP_Instantiate");
		
		if(instantiate == NULL) {
			/* This is an error. */
			std::cout << "Module exists, but does not have AP_Instantiate() . . ." << std::endl;
		}
		else {
			m_instance = instantiate();
			if(m_instance) m_loaded = true;
		}
	}
}

Module::~Module() {
	dlclose(m_moduleHandle);
}

} // namespace Module
} // namespace Monitor
