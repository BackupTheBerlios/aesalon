#include "ConfigurationItemGroup.h"
#include "ConfigurationItemString.h"

namespace Misc {

ConfigurationItemGroup::ConfigurationItemGroup(std::string name) : ConfigurationItem(name, Group) {
	
}

ConfigurationItem *ConfigurationItemGroup::childValue(std::string name) const {
	ConfigurationItem *item = m_itemMap[name];
	if(!item) {
		item = new ConfigurationItemString(name);
		item->setDescription(description());
	}
	return item;
}

} // namespace Misc
