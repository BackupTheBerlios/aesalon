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
	typedef std::map<std::string, std::string> DataMap;
	DataMap m_data;
public:
	virtual ~ConcreteVault() {}
	
	virtual void set(const std::string &key, const std::string &value);
	
	/** Re-implemented.
	*/
	virtual std::string get(const std::string &key);
};

} // namespace Config
} // namespace Monitor

#endif
