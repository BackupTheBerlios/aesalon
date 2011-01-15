/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/module/Loader.cpp

*/

#include "module/Loader.h"
#include "config/ConcreteVault.h"
#include "Coordinator.h"
#include "common/StreamAsString.h"

namespace Monitor {
namespace Module {

ModuleID Loader::m_last = 0;

Loader::Loader() : m_moduleList(NULL) {
	m_vault = Coordinator::instance()->vault();
	m_moduleList = new List();
}

Loader::~Loader() {
	
}

void Loader::loadModules() {
	std::vector<std::string> modules;
	
	m_vault->get("::modules", modules);
	
	for(std::vector<std::string>::iterator i = modules.begin(); i != modules.end(); ++i) {
		loadModule(*i);
	}
}

void Loader::loadModule(std::string name) {
	Module *module = new Module(name);
	m_vault->set(name + ":moduleID", Common::StreamAsString() << ++m_last);
	m_moduleList->addModule(module);
}

} // namespace Module
} // namespace Monitor
