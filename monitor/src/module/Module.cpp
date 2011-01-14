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
#include "common/Preprocessor.h"

namespace Monitor {
namespace Module {

Module::Module(const std::string &moduleName) : m_moduleName(moduleName) {
	m_instance = NULL;
	loadPreprocessor();
	loadPolisher();
}

Module::~Module() {
	dlclose(m_polisherHandle);
}

void Module::loadPolisher() {
	std::string path =
		Coordinator::instance()->vault()->get(m_moduleName + ":root")
		+ Coordinator::instance()->vault()->get(m_moduleName + ":polisherPath");
	
	m_polisherHandle = dlopen(path.c_str(), RTLD_LOCAL | RTLD_NOW);
	if(m_polisherHandle == NULL) return;
	
	Common::MarshallerInterface *(*instantiate)() = NULL;
	
	*(void **)(&instantiate) = dlsym(m_polisherHandle, "AM_InstantiateMarshaller");
	
	if(instantiate == NULL) {
		/* This is an error. */
		std::cout << "Module exists, but does not have exported AM_InstantiateMarshaller() . . ." << std::endl;
		std::cout << "\tDid you forget to place it in an extern \"C\" block?" << std::endl;
	}
	else {
		m_instance = instantiate();
	}
}

void Module::loadPreprocessor() {
	std::string ppPath = Coordinator::instance()->vault()->get(m_moduleName + ":preprocessorPath");
	if(ppPath == "") return;
	std::string path =
		Coordinator::instance()->vault()->get(m_moduleName + ":root") + ppPath;
	
	m_preprocessorHandle = dlopen(path.c_str(), RTLD_LOCAL | RTLD_NOW);
	if(m_preprocessorHandle == NULL) return;
	
	Common::Preprocessor preprocessor;
	
	*(void **)(&preprocessor) = dlsym(m_polisherHandle, "AM_Preprocess");
	
	if(preprocessor == NULL) {
		/* This is an error. */
		std::cout << "Module exists, but does not have AM_Preprocess() . . ." << std::endl;
	}
	else {
		preprocessor(Coordinator::instance()->vault());
	}
}

} // namespace Module
} // namespace Monitor
