#include "ConfigurationItemBoolean.h"

namespace Misc {

ConfigurationItemBoolean::ConfigurationItemBoolean(const std::string &name)
	: ConfigurationItem(name, Boolean) {

}

ConfigurationItemBoolean::~ConfigurationItemBoolean() {
	
}

bool ConfigurationItemBoolean::boolValue() const {
	return m_value;
}

void ConfigurationItemBoolean::setValue(bool value) {
	m_value = value;
}

} // namespace Misc
