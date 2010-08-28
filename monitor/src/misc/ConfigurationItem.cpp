#include "ConfigurationItem.h"
#include "misc/StreamAsString.h"

namespace Misc {

ConfigurationItem::ConfigurationItem(std::string name) : m_name(name) {
}

ConfigurationItem::~ConfigurationItem() {
}

int ConfigurationItem::asInt() const {

}

bool ConfigurationItem::asBool() const {
	if(m_data == "false") return false;
	else if(m_data == "true") return true;
	else {
		throw ConfigurationItemException(Misc::StreamAsString() << "Configuration item \"" << m_name << "\" expects a boolean value.");
	}
}

ConfigurationItemException::ConfigurationItemException(std::string message) : m_message(message) {
	
}

} // namespace Misc
