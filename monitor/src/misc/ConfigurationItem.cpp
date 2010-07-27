#include "ConfigurationItem.h"
#include "LogSystem.h"

namespace Misc {

ConfigurationItem::~ConfigurationItem() {
	
}

ConfigurationItem::ConfigurationItem(const std::string &name, Misc::ConfigurationItem::Type type)
	: m_name(name), m_type(type) {
	
}

bool ConfigurationItem::boolValue() const {
	LogSystem::logConfigurationMessage("Attempted to retreive bool value from non-bool item. Defaulting to false.");
	return false;
}

int ConfigurationItem::intValue() const {
	LogSystem::logConfigurationMessage("Attempted to retreive int value from non-int item. Defaulting to 0.");
	return 0;
}

ConfigurationItem *ConfigurationItem::childValue(std::string name) const {
	LogSystem::logConfigurationMessage("Attempted to retreive child value from non-group item. Defaulting to NULL.");
	return NULL;
}

const std::string &ConfigurationItem::stringValue() const {
	LogSystem::logConfigurationMessage("Attempted to retreive string value from non-string item. Defaulting to empty string.");
	return *new std::string();
}

void ConfigurationItem::setValue(const std::string &value) {
	LogSystem::logConfigurationMessage("Attempted to set string value of non-string item \"" + m_name + "\".");
}

void ConfigurationItem::setValue(const char *value) {
	LogSystem::logConfigurationMessage("Attempted to set string value of non-string item \"" + m_name + "\".");
}

void ConfigurationItem::setValue(bool value) {
	LogSystem::logConfigurationMessage("Attempted to set bool value of non-bool item.");
}

void ConfigurationItem::setValue(int value) {
	LogSystem::logConfigurationMessage("Attempted to set int value of non-int item.");
}

} // namespace Misc
