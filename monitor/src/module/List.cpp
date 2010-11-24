/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/module/List.cpp

*/

#include "module/List.h"

namespace Monitor {
namespace Module {

List::List() {
	m_moduleList.push_back(NULL);
}

List::~List() {
	
}

void List::addModule(Module *module) {
	m_moduleList.push_back(module);
}

Module *List::module(ModuleID id) {
	if(id > m_moduleList.size()) return NULL;
	return m_moduleList[id];
}

} // namespace Module
} // namespace Monitor
