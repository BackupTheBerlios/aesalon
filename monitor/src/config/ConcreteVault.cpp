/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/ConcreteVault.cpp

*/

#include <iostream> // for debugging

#include "config/ConcreteVault.h"

namespace Monitor {
namespace Config {

void ConcreteVault::clear(const std::string &key) {
	m_data.erase(key);
}

void ConcreteVault::set(const std::string &key, const std::string &value) {
	m_data.insert(DataMap::value_type(key, value));
	std::cout << "setting value, new multimap:" << std::endl;
	for(DataMap::const_iterator i = m_data.begin(); i != m_data.end(); ++i) {
        std::cout << "\t* \"" << i->first << "\"->\"" << i->second << "\"\n";
    }
}

std::string ConcreteVault::get(const std::string &key) {
	DataMap::iterator i = m_data.find(key);
	if(i != m_data.end()) return m_data.find(key)->second;
	return "";
}

void ConcreteVault::match(const std::string &pattern, std::vector<KeyPair> &items) {
	for(DataMap::iterator i = m_data.begin(); i != m_data.end(); ++i) {
		if(matches(i->first, pattern)) items.push_back(*i);
	}
}

bool ConcreteVault::matches(const std::string &string, const std::string &pattern) {
	if(string == pattern) return true;
	return false;
}

} // namespace Config
} // namespace Monitor
