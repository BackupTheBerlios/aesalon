#include "ConfigurationModule.h"

namespace Misc {

ConfigurationModule::ConfigurationModule(std::string name) : m_name(name) {
	
}

ConfigurationModule::~ConfigurationModule() {
	
}

void ConfigurationModule::addItem(ConfigurationItem *item) {
	m_itemMap[item->name()] = item;
}

ConfigurationItem *ConfigurationModule::item(std::string name) {
	ConfigurationItem *item = m_itemMap[name];
	
	if(item == NULL) {
		item = new ConfigurationItem(name);
	}
	
	return item;
}

} // namespace Misc
