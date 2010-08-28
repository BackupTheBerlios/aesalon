#include <iostream>
#include "ConfigurationModule.h"

namespace Misc {

ConfigurationModule::ConfigurationModule(std::string name) : m_name(name) {
	
}

ConfigurationModule::~ConfigurationModule() {
	for(ItemMap::iterator i = m_itemMap.begin(); i != m_itemMap.end(); ++i) {
		if(i->second != NULL) delete i->second;
	}
}

void ConfigurationModule::addItem(ConfigurationItem *item) {
	m_itemMap[item->name()] = item;
}

ConfigurationItem *ConfigurationModule::item(std::string name) {
	ConfigurationItem *item = m_itemMap[name];
	
	if(item == NULL) {
		/*std::cout << "Creating new ConfigurationItem \"" << name << "\" . . ." << std::endl;*/
		item = new ConfigurationItem(name);
		m_itemMap[name] = item;
	}
	
	return item;
}

} // namespace Misc
