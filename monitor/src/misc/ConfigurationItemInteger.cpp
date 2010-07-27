#include "ConfigurationItemInteger.h"

namespace Misc {

ConfigurationItemInteger::ConfigurationItemInteger(const std::string &name)
	: ConfigurationItem(name, Integer) {
	
}

ConfigurationItemInteger::~ConfigurationItemInteger() {

}


int ConfigurationItemInteger::intValue() const {
	return m_value;
}

void ConfigurationItemInteger::setValue(int value) {
	m_value = value;
}


} // namespace Misc
