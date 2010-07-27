#ifndef ConfigurationItemString_H
#define ConfigurationItemString_H

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationItemString : public ConfigurationItem {
public:
	ConfigurationItemString(const std::string &name);
	virtual ~ConfigurationItemString();
private:
	std::string m_value;
public:
	virtual const std::string & stringValue() const;
	virtual void setValue(const std::string &value);
	virtual void setValue(const char *value);
};

} // namespace Misc

#endif
