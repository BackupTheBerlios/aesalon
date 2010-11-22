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

Module::Module(const std::string &moduleName) {
	std::string path =
		Coordinator::instance()->vault()->get(moduleName + ":root")
		+ Coordinator::instance()->vault()->get(moduleName + ":polisherPath");
	
	m_moduleHandle = dlopen(path.c_str(), RTLD_LOCAL | RTLD_NOW);
	m_instance = NULL;
	if(m_moduleHandle == NULL) {
		std::cout << "Cannot open module: " << dlerror() << std::endl;
	}
	else {
		Common::PolisherInterface *(*instantiate)() = NULL;
		
		*(void **)(&instantiate) = dlsym(m_moduleHandle, "AP_Instantiate");
		
		if(instantiate == NULL) {
			std::cout << "Module has no function AP_Instantiate() . . ." << std::endl;
		}
		else {
			m_instance = instantiate();
		}
	}
}

Module::~Module() {
	dlclose(m_moduleHandle);
}

} // namespace Module
} // namespace Monitor
