/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/program/Launcher.cpp

*/

#include <iostream>

#include <sys/wait.h>

#include <string.h>
#include <errno.h>

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
	
	/*std::vector<Config::Vault::KeyPair> preloads;
	Coordinator::instance()->vault()->match("*:collectorPath", preloads);*/
	std::vector<std::string> modules;
	
	Coordinator::instance()->vault()->get("::modules", modules);
	
	std::string preload;
	
	for(std::vector<std::string>::iterator i = modules.begin(); i != modules.end(); ++i) {
		std::string moduleRoot = Coordinator::instance()->vault()->get(*i + ":root");
		std::string collectorPath = Coordinator::instance()->vault()->get(*i + ":collectorPath");
		if(collectorPath.length()) {
			if(preload.length()) {
				preload += ":";
			}
			preload += moduleRoot + collectorPath;
		}
	}
	
	pid_t childPid = fork();
	if(childPid == -1) {
		std::cout << "Could not fork . . ." << std::endl;
	}
	else if(childPid == 0) {
		setenv("LD_PRELOAD", preload.c_str(), 1);
		execv(m_argv[0], m_argv);
		std::cout << m_argv[0] << ": " << strerror(errno) << std::endl;
		exit(0);
	}
	else {
		siginfo_t sinfo;
		waitid(P_PID, childPid, &sinfo, WEXITED);
		if(sinfo.si_code == CLD_DUMPED || sinfo.si_code == CLD_KILLED)
			Coordinator::instance()->setReturnValue(128 + sinfo.si_status);
		else Coordinator::instance()->setReturnValue(sinfo.si_status);
	}
}

} // namespace Program
} // namespace Monitor
