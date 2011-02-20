/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/config/Vault.cpp
*/

#include "config/Vault.h"

namespace Config {

Vault::Vault() {

}

void Vault::clear(const std::string &key) {
	m_dataMap.erase(key);
}

void Vault::set(const std::string &key, const std::string &value) {
	m_dataMap.insert(DataMap::value_type(key, value));
}

std::string Vault::get(const std::string &key) {
	DataMap::iterator i = m_dataMap.find(key);
	if(i != m_dataMap.end()) return i->second;
	return "";
}

void Vault::get(const std::string &key, std::vector<std::string> &values) {
	for(DataMap::iterator i = m_dataMap.begin(); i != m_dataMap.end(); ++i) {
		if(i->first == key) values.push_back(i->second);
	}
}

void Vault::match(const std::string &pattern, std::vector<KeyPair> &values) {
	for(DataMap::iterator i = m_dataMap.begin(); i != m_dataMap.end(); ++i) {
		if(matches(i->first, pattern)) values.push_back(*i);
	}
}

bool Vault::matches(const std::string &string, const std::string &pattern) {
	if(string == pattern) return true;
	else if(pattern == "*") return true;
	
	std::string::size_type starPos = pattern.find('*');
	
	if(starPos == std::string::npos) return false;
	if(string.substr(0, starPos) == pattern.substr(0, starPos)) return true;
	
	return false;
}

} // namespace Config
