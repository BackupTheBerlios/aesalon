/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Launcher.cpp

*/

#include <iostream>

#include <sys/wait.h>

#include "program/Launcher.h"

#include "common/AssertionException.h"
#include "common/NYIException.h"
#include "Coordinator.h"
#include "common/StringTo.h"
#include "common/PathSanitizer.h"

namespace Monitor {
namespace Program {

Launcher::Launcher(char **argv) : m_argv(argv) {
	
}

Launcher::~Launcher() {
	
}

void Launcher::startProcess() {
	if(getenv("LD_PRELOAD")) throw Common::NYIException("Pre-existing LD_PRELOAD not handled.");
	
	std::vector<std::string> paths;
	paths.clear();
	Coordinator::instance()->vault()->get("PATH", paths);
	std::vector<Config::Vault::KeyPair> preloads;
	Coordinator::instance()->vault()->match("*:collectorPath", preloads);
	
	std::string preload;
	
	for(std::vector<Config::Vault::KeyPair>::iterator i = preloads.begin(); i != preloads.end(); ++i) {
		std::cout << "preload path \"" << i->first << "/" << i->second << "\" found." << std::endl;
	}
	
	pid_t childPid = fork();
	if(childPid == -1) {
		std::cout << "Could not fork . . ." << std::endl;
	}
	else if(childPid == 0) {
		setenv("LD_PRELOAD", preload.c_str(), 1);
		execv(m_argv[0], m_argv);
		exit(0);
	}
	else {
		siginfo_t sinfo;
		waitid(P_PID, childPid, &sinfo, WEXITED);
	}
}

} // namespace Program
} // namespace Monitor
