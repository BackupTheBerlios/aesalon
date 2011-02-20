/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Launcher.cpp
*/

#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <cstring>

#include "monitor/Launcher.h"

namespace Monitor {

Launcher::Launcher(char **argv) : m_argv(argv) {

}

Launcher::~Launcher() {

}

pid_t Launcher::forkTarget() {
	setupEnvironment();
	
	m_targetPid = fork();
	if(m_targetPid == -1) {
		std::cout << "Could not fork . . ." << std::endl;
		exit(1);
	}
	else if(m_targetPid == 0) {
		execvp(m_argv[0], m_argv);
		std::cout << m_argv[0] << ": " << std::strerror(errno) << std::endl;
		exit(0);
	}
	
	return m_targetPid;
}

void Launcher::setupEnvironment() {
	
}

} // namespace Monitor
