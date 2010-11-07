/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/Store.cpp

*/

#include "config/Store.h"

namespace Monitor {
namespace Config {

Store::Store() {
	
}

Store::~Store() {
	
}

Group *Store::group(const std::string &groupName) {
	Group *group = m_groupMap[groupName];
	if(group == NULL) {
		group = new Group();
		m_groupMap[groupName] = group;
	}
	return group;
}

Item *Store::item(const std::string &groupName, const std::string &itemName) {
	return group(groupName)->item(itemName);
}

Item *Store::findItem(const std::string &groupName, const std::string &itemName) {
	Group *group = m_groupMap[groupName];
	if(group == NULL) return NULL;
	return group->getItem(itemName);
}

Item *Store::item(const std::string &path) {
	std::string::size_type i = path.find('.');
	if(i != std::string::npos) return item(path.substr(0, i), path.substr(i+1));
	return item("", path);
}

} // namespace Config
} // namespace Monitor
