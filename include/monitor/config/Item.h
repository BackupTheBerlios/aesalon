/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/Item.h

*/

#ifndef AesalonMonitor_Config_Item_H
#define AesalonMonitor_Config_Item_H

#include <string>

namespace Monitor {
namespace Config {

class Item {
private:
	std::string m_name;
	std::string m_value;
public:
	Item(const std::string &name);
	~Item();
	
	const std::string &name() const { return m_name; }
	const std::string &value() const { return m_value; }
	void setValue(const std::string &string);
	void setValue(long longValue);
	void setValue(bool boolValue);
	long longValue() const;
	bool boolValue() const;
};

} // namespace Config
} // namespace Monitor

#endif
