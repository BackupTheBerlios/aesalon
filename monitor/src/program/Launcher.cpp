/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Launcher.cpp

*/

#include "program/Launcher.h"

namespace Monitor {
namespace Program {

Launcher::Launcher(Config::Vault *vault, char **argv) : m_vault(vault), m_argv(argv) {
	
}

Launcher::~Launcher() {
	
}

void Launcher::startProcess() {
	
}

} // namespace Program
} // namespace Monitor
