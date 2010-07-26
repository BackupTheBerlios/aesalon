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
	processDefaultConfigFiles();
	processArguments();
}

Configuration::~Configuration() {
	
}

void Configuration::addConfigItem(ConfigurationItem *item) {
	m_configItems[item->name()] = item;
}

void Configuration::processConfigFile(std::string path) {
	path = PathSanitizer::sanitize(path);
	
	std::ifstream file(path.c_str(), std::_S_in);
	if(!file.is_open()) return;
	
	char lineBuffer[8192];
	while(!file.eof()) {
		bool append = false;
		file.getline(lineBuffer, sizeof(lineBuffer));
		std::string line = lineBuffer;
		if(line.length() == 0) continue;
		if(line[0] == '#') continue;
		
		std::string name = line.substr(0, line.find('='));
		std::string content = line.substr(line.find('=') + 1);
		
		/* Handle additions. */
		if(name[name.length()-1] == '+') {
			append = true;
			name.erase(name.length()-1, 1);
		}
		
		ConfigurationItem *configItem = m_configItems[name];
		if(configItem == NULL) {
			LogSystem::logConfigurationMessage(StreamAsString() << "Unknown configuration item \"" << name << "\". Ignoring.");
		}
		else {
			if(configItem->type() == ConfigurationItem::String) {
				if(!append) configItem->setValue(content);
				else configItem->setValue(configItem->stringValue() + content);
			}
			else if(configItem->type() == ConfigurationItem::Boolean) {
				if(append) {
					LogSystem::logConfigurationMessage(
						StreamAsString() << "Cannot append boolean values. Leaving " << name << " unchanged.");
				}
				if(content == "true") configItem->setValue(true);
				else if(content == "false") configItem->setValue(true);
				else {
					LogSystem::logConfigurationMessage(
						StreamAsString() << "Unknown boolean value in " << name << " (" << content << "). Defaulting to false.");
					configItem->setValue(false);
				}
			}
			else if(configItem->type() == ConfigurationItem::Integer) {
				int value;
				std::istringstream converter(content);
				converter >> value;
				if(!append) configItem->setValue(value);
				else configItem->setValue(configItem->intValue() + value);
			}
		}
	}
	file.close();
}

void Configuration::addConfigItems() {
#define String ConfigurationItem::String
#define Boolean ConfigurationItem::Boolean
#define Integer ConfigurationItem::Integer
#define item(name, type, defaultValue, description) \
	m_configItems[name] = new ConfigurationItem(name, type); \
	m_configItems[name]->setValue(defaultValue); \
	m_configItems[name]->setDescription(description); \

#include "ConfigurationItems"

#undef String
#undef Boolean
#undef item
}

void Configuration::processDefaultConfigFiles() {
	processConfigFile(AesalonGlobalConfig);
	processConfigFile(AesalonUserConfig);
	processConfigFile(AesalonLocalConfig);
}

void Configuration::processArguments() {
	bool foundEoo = false;
	
	int index = 0;
	while(m_argv[++index]) {
		std::string indexStr = m_argv[index]; 
		if(indexStr[0] == '-' && indexStr[1] == '-' && !foundEoo) {
			bool append = false;
			indexStr.erase(0, 2);
			if(indexStr.length() == 0) {
				foundEoo = true;
				continue;
			}
			std::string configName;
			std::string configValue;
			if(indexStr.find('=') != std::string::npos) {
				configName = indexStr.substr(0, indexStr.find('='));
				configValue = indexStr.substr(indexStr.find('=')+1);
			}
			else {
				configName = indexStr;
				configValue = "true";
			}
			
			if(configName[configName.length()-1] == '+') {
				append = true;
				configName.erase(configName.length()-1, 1);
			}
			
			ConfigurationItem *item = m_configItems[configName];
			
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
			}
		}
		else {
			m_programArguments.push_back(indexStr);
		}
	}
	if(m_programArguments.size()) m_filename = m_programArguments[0];
}

} // namespace Misc
