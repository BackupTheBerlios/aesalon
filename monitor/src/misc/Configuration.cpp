#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "Configuration.h"
#include "PathSanitizer.h"
#include "LogSystem.h"
#include "StreamAsString.h"

namespace Misc {

Configuration::Configuration(char *argv[]) : m_argv(argv) {
	addConfigItems();
	processConfigFiles();
}

Configuration::~Configuration() {
	
}

void Configuration::addConfigItems() {
#define String ConfigurationItem::String
#define Boolean ConfigurationItem::Boolean
#define Integer ConfigurationItem::Integer
#define item(name, type, defaultValue, description) \
	m_configItems[name] = new ConfigurationItem(name, type); \
	m_configItems[name]->setValue(defaultValue); \
	m_configItems[name]->setDescription(description);
	
#include "ConfigurationItems"
	
#undef String
#undef Boolean
#undef item
}

void Configuration::processConfigFiles() {
	processConfigFile(AesalonGlobalConfig);
	processConfigFile(AesalonUserConfig);
	processConfigFile(AesalonLocalConfig);
}

void Configuration::processConfigFile(std::string path) {
	path = PathSanitizer::santize(path);
	
	std::ifstream file(path.c_str(), std::_S_in);
	if(!file.is_open()) return;
	
	char lineBuffer[8192];
	while(!file.eof()) {
		file.getline(lineBuffer, sizeof(lineBuffer));
		std::string line = lineBuffer;
		if(line.length() == 0) continue;
		if(line[0] == '#') continue;
		
		std::string name = line.substr(0, line.find('='));
		std::string content = line.substr(line.find('=') + 1);

		ConfigurationItem *configItem = m_configItems[name];
		if(configItem == NULL) {
			LogSystem::logConfigurationMessage(StreamAsString() << "Unknown configuration item \"" << name << "\". Ignoring.");
		}
		else {
			if(configItem->type() == ConfigurationItem::String) configItem->setValue(content);
			else if(configItem->type() == ConfigurationItem::Boolean) {
				if(content == "true") configItem->setValue(true);
				else if(content == "false") configItem->setValue(true);
				else {
					LogSystem::logConfigurationMessage(
						StreamAsString() << "Unknown boolean value in " << name << " (" << content << "). Defaulting to false.");
					configItem->setValue(false);
				}
			}
		}
	}
	file.close();
}

void Configuration::processArguments() {
	
}

} // namespace Misc
