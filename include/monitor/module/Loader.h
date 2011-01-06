/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/module/Loader.h

*/

#ifndef AesalonMonitor_Module_Loader_H
#define AesalonMonitor_Module_Loader_H

#include <string>

#include "common/ModuleID.h"
#include "module/List.h"

namespace Monitor {

namespace Config {
class ConcreteVault;
} // namespace Config

namespace Module {

class Loader {
private:
	static ModuleID m_last;
	Config::ConcreteVault *m_vault;
	List *m_moduleList;
public:
	Loader();
	~Loader();
	
	List *moduleList() const { return m_moduleList; }
	
	void loadModules();
private:
	void loadModule(std::string name);
};

} // namespace Module
} // namespace Monitor

#endif
