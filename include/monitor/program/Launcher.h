/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/Launcher.h

*/

#ifndef AesalonMonitor_Program_Launcher_H
#define AesalonMonitor_Program_Launcher_H

#include "config/Vault.h"

namespace Monitor {
namespace Program {

class Launcher {
private:
	Config::Vault *m_vault;
	char **m_argv;
public:
	Launcher(Config::Vault *vault, char **argv);
	~Launcher();
	
	void startProcess();
};

} // namespace Program
} // namespace Monitor

#endif
