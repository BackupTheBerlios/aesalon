#include <fstream>

#include "Configuration.h"

namespace Misc {

Configuration::Configuration() {
	
}

Configuration::~Configuration() {
	for(ModuleMap::iterator i = m_moduleMap.begin(); i != m_moduleMap.end(); i ++) {
		if(*i != NULL) delete *i;
	}
}

ConfigurationModule *Configuration::module(const std::string &name) {
	ConfigurationModule *module = m_moduleMap[name];
	if(module == NULL) {
		module = new ConfigurationModule(name);
	}
	return module;
}

void Configuration::processFile(const std::string &path) {
	std::fstream file(path, std::ios_base::in);
	
	if(!file.is_open()) return;
	
	file.close();
}

void Configuration::processArgv(const char **argv) {
	
}

void Configuration::addLaunchArgument(const std::string &argument) {
	m_launchArguments += argument;
	m_launchArguments += " ";
}

} // namespace Misc
