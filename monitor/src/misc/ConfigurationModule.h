#ifndef ConfigurationModule_H
#define ConfigurationModule_H

#include <string>
#include <map>

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationModule {
public:
	ConfigurationModule(std::string name);
	~ConfigurationModule();
private:
	std::string m_name;
	typedef std::map<std::string, ConfigurationItem *> ItemMap;
	ItemMap m_itemMap;
public:
	const std::string &name() const { return m_name; }
	
	void addItem(ConfigurationItem *item);
	ConfigurationItem *item(std::string name);
};

} // namespace Misc

#endif
