/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/Group.h

*/

#ifndef AesalonMonitor_Config_Group_H
#define AesalonMonitor_Config_Group_H

#include <string>
#include <map>

#include "Item.h"

namespace Monitor {
namespace Config {

class Group {
public:
	typedef std::map<std::string, Item *> ItemMap;
private:
	ItemMap m_itemMap;
public:
	Group();
	~Group();
	
	const ItemMap &itemMap() const { return m_itemMap; }
	
	void addItem(Item *item);
	Item *item(const std::string &name);
	Item *getItem(const std::string &name);
};

} // namespace Config
} // namespace Monitor

#endif
