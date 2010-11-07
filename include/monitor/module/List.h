/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/module/List.h

*/

#ifndef AesalonMonitor_Module_List_H
#define AesalonMonitor_Module_List_H

#include <vector>

#include "Module.h"

namespace Monitor {
namespace Module {

class List {
private:
	typedef std::vector<Module *> ModuleList;
	ModuleList m_moduleList;
public:
	List();
	~List();
};

} // namespace Module
} // namespace Monitor

#endif
