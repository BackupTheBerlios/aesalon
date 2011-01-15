/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/module/List.cpp

*/

#include <iostream>

#include "module/List.h"
#include "Coordinator.h"
#include "common/StringTo.h"

namespace Monitor {
namespace Module {

List::List() {
	m_moduleList.push_back(NULL);
}

List::~List() {
	
}

void List::addModule(Module *module) {
	ModuleID moduleID = Common::StringTo<ModuleID>(
		Coordinator::instance()->vault()->get(module->moduleName() + ":moduleID"));
	
	if(moduleID >= m_moduleList.size()) {
		m_moduleList.resize(module->moduleID()+1);
	}
	m_moduleList[moduleID] = module;
}

Module *List::module(ModuleID id) {
	if(id > m_moduleList.size()) return NULL;
	return m_moduleList[id];
}

Module *List::module(std::string name) {
	for(ModuleList::iterator i = m_moduleList.begin(); i != m_moduleList.end(); ++i) {
		if((*i)->moduleName() == name) return *i;
	}
	return NULL;
}

} // namespace Module
} // namespace Monitor
