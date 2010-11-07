/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/Vault.h

*/

#ifndef AesalonMonitor_Config_Vault_H
#define AesalonMonitor_Config_Vault_H

#include <string>

namespace Monitor {
namespace Config {

class Vault {
public:
	virtual ~Vault() {}
	
	/** Returns the last value that @a key maps to. For all values, use @c match().
	*/
	virtual std::string get(const std::string &key) = 0;
	//virtual void match(const std::string &pattern, std::vector<std::string> &keys) = 0;
};

} // namespace Config
} // namespace Monitor

#endif
