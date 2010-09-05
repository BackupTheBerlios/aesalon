#include "ModuleMapper.h"

ModuleMapper::ModuleMapper() {
	m_moduleList.append(NULL);
}

ModuleMapper::~ModuleMapper() {
	
}

void ModuleMapper::processPacket(DataPacket *packet) {
	/* Module ID #0 is the interface; thus if it it's not the collector interface . . . */
	if(packet->dataSource.moduleID != 0) module(packet->dataSource.moduleID)->processIncoming(packet);
	/* If the size is nonzero, then it is a module loading notification; load the corresponding module. */
	else if(packet->dataSize != 0) {
		const char *name = (const char *)packet->data;
		loadModule(name);
	}
	/* If the size is zero, then the packet is a heartbeat. */
	else {
		for(int i = 1; i < m_moduleList.size(); i ++) {
			m_moduleList[i]->controller()->dataCache()->updateHeartbeatTimestamp(packet->dataSource.timestamp);
		}
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
