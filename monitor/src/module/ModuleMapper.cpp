#include "ModuleMapper.h"

namespace Module {

ModuleMapper::ModuleMapper() {
	
}

ModuleMapper::~ModuleMapper() {
	
}

Module *ModuleMapper::module(uint16_t moduleID) {
	return m_moduleVector[moduleID];
}

Module *ModuleMapper::module(const std::string &name) {
	return m_moduleMap[name];
}

void ModuleMapper::registerModule(Module *module) {
	if(module->moduleID() > (m_moduleVector.size() + 1)) {
		m_moduleVector.resize(module->moduleID() + 1);
	}
	m_moduleMap[module->moduleName()] = module;
}

} // namespace Module
