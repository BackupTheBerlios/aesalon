#ifndef ConfigurationItem_H
#define ConfigurationItem_H

#include <string>

namespace Misc {

class ConfigurationItem {
public:
	ConfigurationItem(std::string name);
	~ConfigurationItem();
private:
	std::string m_name;
	std::string m_data;
public:
	const std::string &name() const { return m_name; }
	void setData(const std::string &data) { m_data = data; }
	const std::string &data() const { return m_data; }
	
	int asInt() const;
	bool asBool() const;
};

class ConfigurationItemException {
public:
	ConfigurationItemException(std::string message);
	~ConfigurationItemException();
private:
	std::string m_message;
public:
	const std::string &message() const { return m_message; }
};

} // namespace Misc

#endif
