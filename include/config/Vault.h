/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/config/Vault.h
*/

#ifndef AesalonConfig_Vault_H
#define AesalonConfig_Vault_H

#include <map>
#include <string>
#include <vector>

namespace Config {

class Vault {
public:
	typedef std::pair<std::string, std::string> KeyPair;
private:
	typedef std::multimap<std::string, std::string> DataMap;
	DataMap m_dataMap;
public:
	Vault();
	
	void clear(const std::string &key);
	void set(const std::string &key, const std::string &value);
	
	std::string get(const std::string &key);
	
	void get(const std::string &key, std::vector<std::string> &values);
	
	void match(const std::string &pattern, std::vector<KeyPair> &values);
private:
	bool matches(const std::string &string, const std::string &pattern);
};

} // namespace Config

#endif

