/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/Group.cpp

*/

#include "config/Group.h"

namespace Monitor {
namespace Config {

Group::Group() {

}

Group::~Group() {

}

void Group::addItem(Item *item) {
	m_itemMap[item->name()] = item;
}

Item *Group::item(const std::string &item) {
	Item *i = m_itemMap[item];
	if(i == NULL) {
		i = new Item(item);
		m_itemMap[item] = i;
	}
	return i;
}

} // namespace Config
} // namespace Monitor
