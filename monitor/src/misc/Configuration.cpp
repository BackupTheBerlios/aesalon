#include "Configuration.h"

namespace Misc {

Configuration::Configuration(char *argv[]) : m_argv(argv) {
	addConfigItems();
}

Configuration::~Configuration() {
	
}

void Configuration::addConfigItems() {
#define String ConfigurationItem::String
#define Boolean ConfigurationItem::Boolean
#define item(name, type, defaultValue) \
	m_configItems[name] = new ConfigurationItem(name, type); \
	m_configItems[name]->setValue(defaultValue);
	
#include "ConfigurationItems"
	
#undef String
#undef Boolean
#undef item
}

void Configuration::processConfigFiles() {
	
}

void Configuration::processArguments() {
	
}

} // namespace Misc
