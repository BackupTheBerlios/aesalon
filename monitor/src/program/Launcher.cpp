/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Launcher.cpp

*/

#include <iostream>

#include "program/Launcher.h"

#include "common/AssertionException.h"
#include "Coordinator.h"
#include "common/StringTo.h"

namespace Monitor {
namespace Program {

Launcher::Launcher(char **argv) : m_argv(argv) {
	
}

Launcher::~Launcher() {
	
}

void Launcher::startProcess() {
	std::string s = Coordinator::instance()->vault()->get("shmSize");
	std::cout << "shmSize: \"" << s << "\"\n";
	std::cout << "size:" << Common::StringTo<int>(Coordinator::instance()->vault()->get("shmSize")) << std::endl;
	throw Common::AssertionException("Size of shared memory must be a nonzero multiple of four.");
}

} // namespace Program
} // namespace Monitor
