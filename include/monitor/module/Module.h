/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/module/Module.h

*/

#ifndef AesalonMonitor_Module_Module_H
#define AesalonMonitor_Module_Module_H

#include <string>

#include "config/Vault.h"
#include "common/PolisherInterface.h"

namespace Monitor {
namespace Module {

class Module {
private:
	std::string m_moduleName;
	void *m_moduleHandle;
	Common::PolisherInterface *m_instance;
public:
	Module(const std::string &moduleName);
	~Module();
};

} // namespace Module
} // namespace Monitor

#endif
