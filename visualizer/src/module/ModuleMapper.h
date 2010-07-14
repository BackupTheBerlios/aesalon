#ifndef ModuleMapper_H
#define ModuleMapper_H

#include <QList>
#include <QMap>

#include "Module.h"

class ModuleMapper {
public:
	ModuleMapper();
	~ModuleMapper();
private:
	QList<Module *> m_moduleList;
	QMap<QString, Module *> m_moduleMap;
public:
	Module *module(quint16 moduleID);
	
	void processPacket(DataPacket *packet);
private:
	void loadModule(const char *name);
};

#endif
