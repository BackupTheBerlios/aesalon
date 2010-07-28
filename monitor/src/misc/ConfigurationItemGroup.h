#ifndef ConfigurationItemGroup_H
#define ConfigurationItemGroup_H

#include <string>
#include <map>

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationItemGroup : public ConfigurationItem {
public:
	ConfigurationItemGroup(std::string name);
	virtual ~ConfigurationItemGroup();
private:
	typedef std::map<std::string, ConfigurationItem *> ItemMap;
	/* NOTE: this mutable is probably not a good idea. */
	mutable ItemMap m_itemMap;
public:
	virtual ConfigurationItem *childValue(std::string name) const;
};

} // namespace Misc

#endif
