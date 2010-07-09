#ifndef ConfigurationItem_H
#define ConfigurationItem_H

#include <string>

namespace Misc {

class ConfigurationItem {
public:
	enum Type {
		Boolean,
		String,
		Integer
	};
	ConfigurationItem(const std::string &name, Type type);
private:
	std::string m_name;
	Type m_type;
	std::string m_stringValue;
	bool m_boolValue;
	int m_intValue;
	std::string m_description;
public:
	const std::string &name() const { return m_name; }
	Type type() const { return m_type; }
	const std::string &stringValue() const { return m_stringValue; }
	bool boolValue() const { return m_boolValue; }
	int intValue() const { return m_intValue; }
	
	const std::string &description() const { return m_description; }
	void setDescription(const std::string &description) { m_description = description; }
	
	void setValue(const std::string &value) { m_stringValue = value; }
	void setValue(bool value) { m_boolValue = value; }
	void setValue(int value) { m_intValue = value; }
};

} // namespace Misc

#endif
