/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/Store.h

*/

#ifndef AesalonMonitor_Config_Store_H
#define AesalonMonitor_Config_Store_H

#include <string>
#include <map>

#include "Item.h"
#include "Group.h"

namespace Monitor {
namespace Config {

class Store {
private:
	typedef std::map<std::string, Group *> GroupMap;
	GroupMap m_groupMap;
	
public:
	Store();
	~Store();
	
	Group *group(const std::string &groupName);
	Item *item(const std::string &groupName, const std::string &itemName);
	Item *item(const std::string &path);
};

} // namespace Config
} // namespace Monitor

#endif
