#include <sstream>

#include "ConfigurationItem.h"
#include "misc/StreamAsString.h"

namespace Misc {

ConfigurationItem::ConfigurationItem(std::string name) : m_name(name) {
}

ConfigurationItem::~ConfigurationItem() {
}

int ConfigurationItem::asInt() const {
	std::stringstream stream;
	stream << m_data;
	int value;
	stream >> value;
	return value;
}

bool ConfigurationItem::asBool() const {
	if(m_data == "false" || m_data == "") return false;
	else if(m_data == "true") return true;
	else {
		throw ConfigurationItemException(Misc::StreamAsString() << "Configuration item \"" << m_name << "\" expects a boolean value.");
	}
}

ConfigurationItemException::ConfigurationItemException(std::string message) : m_message(message) {
	
}

ConfigurationItemException::~ConfigurationItemException() {

}

} // namespace Misc
