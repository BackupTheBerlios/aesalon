/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/Coordinator.h

*/

#ifndef AesalonMonitor_Coordinator_H
#define AesalonMonitor_Coordinator_H

#include "config/Vault.h"
#include "module/List.h"

namespace Monitor {

class Coordinator {
private:
	static Coordinator *m_instance;
	char **m_argv;
	Config::Vault *m_vault;
	Module::List *m_moduleList;
public:
	static Coordinator *instance() { return m_instance; }
	Coordinator(char **argv);
	~Coordinator();
	
	char **argv() const { return m_argv; }
	Config::Vault *vault() const { return m_vault; }
	Module::List *moduleList() const { return m_moduleList; }
	
	void run();
private:
	void parseConfigs();
	std::string moduleRoot(const std::string &moduleName);
	void usage(bool displayHelp);
};

} // namespace Monitor

#endif
