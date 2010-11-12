/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/ConcreteVault.h

*/

#ifndef AesalonMonitor_Config_ConcreteVault_H
#define AesalonMonitor_Config_ConcreteVault_H

#include <map>

#include "Vault.h"

namespace Monitor {
namespace Config {

class ConcreteVault : public Vault {
private:
	typedef std::multimap<std::string, std::string> DataMap;
	DataMap m_data;
public:
	virtual ~ConcreteVault() {}
	
	virtual void clear(const std::string &key);
	virtual void set(const std::string &key, const std::string &value);
	
	/** Re-implemented.
	*/
	virtual std::string get(const std::string &key);
	
	/** Re-implemented.
	*/
	virtual void match(const std::string &pattern, std::vector<KeyPair> &items);
private:
	/** Returns true if @a string matches @a pattern.
	*/
	bool matches(const std::string &string, const std::string &pattern);
};

} // namespace Config
} // namespace Monitor

#endif
