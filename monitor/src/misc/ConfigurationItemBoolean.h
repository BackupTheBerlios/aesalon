#ifndef ConfigurationItemBoolean_H
#define ConfigurationItemBoolean_H

#include "ConfigurationItem.h"

namespace Misc {

class ConfigurationItemBoolean : public ConfigurationItem {
public:
	ConfigurationItemBoolean(const std::string &name);
	virtual ~ConfigurationItemBoolean();
private:
	bool m_value;
public:
	virtual bool boolValue() const;
	virtual void setValue(bool value);
};

} // namespace Misc

#endif
