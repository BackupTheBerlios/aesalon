#ifndef ModuleMapper_H
#define ModuleMapper_H

#include <string>
#include <map>
#include <vector>

#include "DataTypes.h"
#include "Module.h"

namespace Module {

class ModuleMapper {
public:
	ModuleMapper();
	~ModuleMapper();
private:
	std::vector<Module *> m_moduleVector;
	std::map<std::string, Module *> m_moduleMap;
public:
	Module *module(uint16_t moduleID);
	Module *module(const std::string &name);
	
	void loadModule(char *moduleName);
};

} // namespace Module

#endif
