#include "ModuleMapper.h"

ModuleMapper::ModuleMapper() {
	
}

ModuleMapper::~ModuleMapper() {
	
}

void ModuleMapper::processPacket(DataPacket *packet) {
	if(packet->dataSource.moduleID != 0) module(packet->dataSource.moduleID)->processIncoming(packet);
	else {
		const char *name = (const char *)packet->data;
		loadModule(name);
	}
}

Module *ModuleMapper::module(quint16 moduleID) {
	return m_moduleList[moduleID];
}

void ModuleMapper::loadModule(const char *name) {
	Module *module = new Module(name);
	m_moduleList.append(module);
	m_moduleMap[name] = module;
}
