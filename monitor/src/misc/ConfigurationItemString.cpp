#include "ConfigurationItemString.h"

namespace Misc {

ConfigurationItemString::ConfigurationItemString(const std::string &name): ConfigurationItem(name, String) {

}

ConfigurationItemString::~ConfigurationItemString() {

}

const std::string &ConfigurationItemString::stringValue() const {
	return m_value;
}

void ConfigurationItemString::setValue(const std::string &value) {
	m_value = value;
}

void ConfigurationItemString::setValue(const char *value) {
	m_value = value;
}


} // namespace Misc
