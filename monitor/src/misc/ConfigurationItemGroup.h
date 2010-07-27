#ifndef ConfigurationItemGroup_H
#define ConfigurationItemGroup_H

#include <string>
#include <map>

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationItemGroup : public ConfigurationItem {
public:
	ConfigurationItemGroup(std::string name);
	
private:
	typedef std::map<std::string, ConfigurationItem *> ItemMap;
public:
	
};

} // namespace Misc

#endif
