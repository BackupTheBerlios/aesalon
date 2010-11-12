/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/ArgumentParser.h

*/

#ifndef AesalonMonitor_Config_ArgumentParser_H
#define AesalonMonitor_Config_ArgumentParser_H

#include "Vault.h"

namespace Monitor {
namespace Config {

class ConcreteVault;

class ArgumentParser {
public:
	/**
		@return The argc corresponding to the first argv element that was not processed.
	*/
	int parse(ConcreteVault *vault, char **argv);
};

} // namespace Config
} // namespace Monitor

#endif
