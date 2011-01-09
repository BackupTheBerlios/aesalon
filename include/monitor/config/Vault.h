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
#include <vector>

namespace Monitor {
namespace Config {

class Vault {
public:
	typedef std::pair<std::string, std::string> KeyPair;
	virtual ~Vault() {}
	
	/** Clears @a key to have no value.
	*/
	virtual void clear(const std::string &key) = 0;
	/** Sets @a key to @a value.
	*/
	virtual void set(const std::string &key, const std::string &value) = 0;
	
	/** Returns the last value that @a key maps to.
	*/
	virtual std::string get(const std::string &key) = 0;
	/** Stores all the values of @a key in @a values.
	*/
	virtual void get(const std::string &key, std::vector<std::string> &values) = 0;
	/** Places the names of all strings that match @a pattern into @a keys. Note that
		@a keys will not be cleared.
	*/
	virtual void match(const std::string &pattern, std::vector<KeyPair> &items) = 0;
};

} // namespace Config
} // namespace Monitor

#endif
