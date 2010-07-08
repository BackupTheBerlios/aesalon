#include "ConfigurationItem.h"

namespace Misc {

ConfigurationItem::ConfigurationItem(const std::string &name, Misc::ConfigurationItem::Type type)
	: m_name(name), m_type(type), m_stringValue(""), m_boolValue(false) {
	
}

} // namespace Misc
