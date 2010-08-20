#include "ConfigurationItemGroup.h"
#include "ConfigurationItemString.h"

namespace Misc {

ConfigurationItemGroup::ConfigurationItemGroup(std::string name) : ConfigurationItem(name, Group) {
	
}

ConfigurationItemGroup::~ConfigurationItemGroup() {
	for(ItemMap::iterator i = m_itemMap.begin(); i != m_itemMap.end(); i ++) {
		if(i->second != NULL) delete i->second;
	}
}

ConfigurationItem *ConfigurationItemGroup::childValue(std::string name) const {
	ConfigurationItem *item = m_itemMap[name];
	if(!item) {
		item = new ConfigurationItemString(name);
		item->setDescription(description());
		m_itemMap[name] = item;
	}
	return item;
}

} // namespace Misc
