#ifndef ConfigurationItemInteger_H
#define ConfigurationItemInteger_H

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationItemInteger : public ConfigurationItem {
public:
	ConfigurationItemInteger(const std::string &name);
	virtual ~ConfigurationItemInteger();
private:
	int m_value;
public:
	virtual int intValue() const;
	virtual void setValue(int value);
};

} // namespace Misc

#endif
