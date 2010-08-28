#ifndef ConfigurationModule_H
#define ConfigurationModule_H

#include <string>
#include <map>

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationModule {
public:
	typedef std::map<std::string, ConfigurationItem *> ItemMap;
	
	ConfigurationModule(std::string name);
	~ConfigurationModule();
private:
	std::string m_name;
	ItemMap m_itemMap;
public:
	const std::string &name() const { return m_name; }
	
	void addItem(ConfigurationItem *item);
	ConfigurationItem *item(std::string name);
	
	const ItemMap &itemMap() const { return m_itemMap; }
};

} // namespace Misc

#endif
