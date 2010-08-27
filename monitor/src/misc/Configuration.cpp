#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "Configuration.h"
#include "PathSanitizer.h"
#include "LogSystem.h"
#include "StreamAsString.h"
#include "ConfigurationItemBoolean.h"
#include "ConfigurationItemInteger.h"
#include "ConfigurationItemString.h"
#include "ConfigurationItemGroup.h"

namespace Misc {

Configuration::Configuration(char *argv[]) : m_argv(argv) {
	addConfigItems();
	processDefaultConfigFiles();
	processArgumentsModuleList();
	processModuleConfigFiles();
	processArguments();
}

Configuration::~Configuration() {
	for(ConfigItems::iterator i = m_configItems.begin(); i != m_configItems.end(); i ++) {
		if(i->second) delete i->second;
	}
}

void Configuration::addConfigItem(ConfigurationItem *item) {
	m_configItems[item->name()] = item;
}

void Configuration::processConfigFile(std::string path) {
	path = PathSanitizer::sanitize(path);
	
	std::ifstream file(path.c_str(), std::_S_in);
	if(!file.is_open()) return;
	
	char lineBuffer[8192];
	memset(lineBuffer, 0, sizeof(lineBuffer));
	while(!file.eof()) {
		file.getline(lineBuffer, sizeof(lineBuffer));
		std::string line;
		line.append(lineBuffer);
		if(line.length() == 0) continue;
		if(line[0] == '#') continue;
		
		processItem(line);
		
		continue;
	}
	file.close();
}

void Configuration::addConfigItems() {
#define String ConfigurationItemString
#define Boolean ConfigurationItemBoolean
#define Integer ConfigurationItemInteger
#define item(name, type, defaultValue, description) \
	m_configItems[name] = new type(name); \
	m_configItems[name]->setValue(defaultValue); \
	m_configItems[name]->setDescription(description);
#define group(name, description) \
	m_configItems[name] = new ConfigurationItemGroup(name); \
	m_configItems[name]->setDescription(description);

#include "ConfigurationItems"

#undef String
#undef Boolean
#undef Integer
#undef item
}

void Configuration::processDefaultConfigFiles() {
	processConfigFile(AesalonGlobalConfig);
	processConfigFile(AesalonUserConfig);
	processConfigFile(AesalonLocalConfig);
}

void Configuration::processModuleConfigFiles() {
	std::string moduleList = m_configItems["modules"]->stringValue();
	
	if(moduleList.length() == 0) return;
	
	std::string::size_type offset = 0;
	std::string::size_type index = moduleList.find(":");
	
	do {
		std::string module = moduleList.substr(offset, index);
		
		std::string modulePath = Misc::PathSanitizer::findFromPaths(module, m_configItems["search-path"]->stringValue());
		
		if(modulePath.length()) {
			/*std::cout << "processing " << modulePath << "/monitor.conf . . ." << std::endl;*/
			processConfigFile(modulePath + "/monitor.conf");
			m_configItems["_module-path"]->childValue(module)->setValue(modulePath);
		}
		else {
			LogSystem::logConfigurationMessage("Could not find module \"" + module + "\". Module will not be loaded.");
		}
		
		offset += module.length();
		offset ++;
	} while((index = moduleList.find(':', offset)) != std::string::npos);
}

void Configuration::processArguments() {
	bool foundEoo = false;
	
	int index = 0;
	while(m_argv[++index]) {
		std::string indexStr = m_argv[index]; 
		if(indexStr[0] == '-' && indexStr[1] == '-' && !foundEoo) {
			indexStr.erase(0, 2);
			if(indexStr.length() == 0) {
				foundEoo = true;
				continue;
			}
			/* Handle "modules" specially. */
			if(indexStr == "modules") continue;
			processItem(indexStr);
		}
		else {
			m_programArguments.push_back(indexStr);
		}
	}
	if(m_programArguments.size()) m_filename = m_programArguments[0];
}

void Configuration::processArgumentsModuleList() {
	bool foundEoo = false;
	
	int index = 0;
	while(m_argv[++index]) {
		std::string indexStr = m_argv[index]; 
		if(indexStr[0] == '-' && indexStr[1] == '-' && !foundEoo) {
			indexStr.erase(0, 2);
			if(indexStr.length() == 0) {
				foundEoo = true;
				continue;
			}
			/* Handle "modules" specially. */
			if(indexStr != "modules") continue;
			processItem(indexStr);
		}
		else {
			m_programArguments.push_back(indexStr);
		}
	}
	if(m_programArguments.size()) m_filename = m_programArguments[0];
}

void Configuration::processItem(std::string itemStr) {
	bool append = false;
	std::string configName;
	std::string configValue;
	if(itemStr.find('=') != std::string::npos) {
		configName = itemStr.substr(0, itemStr.find('='));
		configValue = itemStr.substr(itemStr.find('=')+1);
	}
	else {
		configName = itemStr;
		configValue = "true";
	}
	
	if(configName[configName.length()-1] == '+') {
		append = true;
		configName.erase(configName.length()-1, 1);
	}
	
	ConfigurationItem *item = m_configItems[configName];
	
	if(item == NULL) {
		/* Try the group list first. */
		/* NOTE: this is a VERY hackish approach, but I can't think of a better way to do it at the moment. */
		/* TODO: fix this up a little, possible handle groups specially? */
		std::string name = configName;
		std::string childName = "";
		std::string::size_type index = 0;
		while((index = name.rfind('-')) != std::string::npos) {
			childName += name.substr(index+1);
			name.erase(index);
			item = m_configItems[name];
			if(item && item->type() == ConfigurationItem::Group) {
				ConfigurationItem *childItem = item->childValue(childName);
				item = childItem;
				break;
			}
		}
		
		/* if item is still NULL . . . */
		if(item == NULL) {
			LogSystem::logConfigurationMessage(StreamAsString() << "Unknown config item encountered: " << configName);
			return;
		}
	}
	
	switch(item->type()) {
		case ConfigurationItem::Boolean: {
			if(append) {
				LogSystem::logConfigurationMessage(
					StreamAsString() << "Cannot append boolean values. Leaving " << configName << " unchanged.");
			}
			if(configValue == "true") item->setValue(true);
			else if(configValue == "false") item->setValue(false);
			else {
				LogSystem::logConfigurationMessage(StreamAsString() << "Unknown boolean value in " << configName
					<< " (" << configValue << "). Defaulting to false.");
				item->setValue(false);
			}
			break;
		}
		case ConfigurationItem::String: {
			if(!append) item->setValue(configValue);
			else item->setValue(item->stringValue() + configValue);
			break;
		}
		case ConfigurationItem::Integer: {
			int value;
			std::istringstream converter(configValue);
			converter >> value;
			if(!append) item->setValue(value);
			else item->setValue(item->intValue() + value);
			break;
		}
		case ConfigurationItem::Group: {
			LogSystem::logConfigurationMessage(
				StreamAsString() << "Attempting to set the value of a configuration group."
				<< " For reference, this is not a good idea. Append a subname to the end of "
				<< configName);
			break;
		}
	}
}

} // namespace Misc
