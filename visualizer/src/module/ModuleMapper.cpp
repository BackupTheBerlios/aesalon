#include "ModuleMapper.h"

ModuleMapper::ModuleMapper() {
	m_moduleList.append(NULL);
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
	/*qDebug("Asked for module pointer for ID %i . . .", moduleID);*/
	return m_moduleList[moduleID];
}

Module *ModuleMapper::module(QString moduleName) {
	return m_moduleMap[moduleName];
}

void ModuleMapper::loadModule(const char *name) {
	qDebug("Asked to load module \"%s\" . . .", name);
	Module *module = new Module(name);
	m_moduleList.append(module);
	m_moduleMap[name] = module;
	qDebug("Module loaded. ID: %i", m_moduleList.size() - 1);
}
