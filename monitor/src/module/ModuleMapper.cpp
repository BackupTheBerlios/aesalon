#include <iostream>

#include "ModuleMapper.h"
#include "misc/PathSanitizer.h"
#include "Initializer.h"

namespace Module {

ModuleMapper::ModuleMapper() {
	m_moduleVector.resize(1);
}

ModuleMapper::~ModuleMapper() {
	
}

Module *ModuleMapper::module(uint16_t moduleID) {
	if(moduleID >= m_moduleVector.size()) return NULL;
	return m_moduleVector[moduleID];
}

Module *ModuleMapper::module(const std::string &name) {
	return m_moduleMap[name];
}

void ModuleMapper::loadModule(char *moduleName) {
	Module *module = new Module(m_moduleVector.size(), moduleName);
	m_moduleVector.push_back(module);
	m_moduleMap[moduleName] = module;
}

} // namespace Module
