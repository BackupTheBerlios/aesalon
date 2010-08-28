#include "ConfigurationModule.h"

namespace Misc {

ConfigurationModule::ConfigurationModule(std::string name) : m_name(name) {
	
}

ConfigurationModule::~ConfigurationModule() {
	
}

void ConfigurationModule::addItem(ConfigurationItem *item) {
	m_itemMap[item->name()] = item;
}

void ConfigurationModule::item(std::string name) {
	return m_itemMap[name];
}

} // namespace Misc
