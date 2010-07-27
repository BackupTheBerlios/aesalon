#ifndef ConfigurationItem_H
#define ConfigurationItem_H

#include <string>

namespace Misc {

class ConfigurationItem {
public:
	enum Type {
		Boolean,
		String,
		Integer,
		Group
	};
	
	virtual ~ConfigurationItem();
protected:
	ConfigurationItem(const std::string &name, Type type);
private:
	std::string m_name;
	Type m_type;
	std::string m_description;
public:
	const std::string &name() const { return m_name; }
	Type type() const { return m_type; }
	virtual const std::string &stringValue() const;
	virtual bool boolValue() const;
	virtual int intValue() const;
	virtual ConfigurationItem *childValue(std::string name) const;
	
	const std::string &description() const { return m_description; }
	void setDescription(const std::string &description) { m_description = description; }
	
	virtual void setValue(const std::string &value);
	virtual void setValue(const char *value);
	virtual void setValue(bool value);
	virtual void setValue(int value);
};

} // namespace Misc

#endif
