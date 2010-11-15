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
#include "common/StreamAsString.h"

namespace Monitor {
namespace Program {

Launcher::Launcher(char **argv) : m_argv(argv) {
	
}

Launcher::~Launcher() {
	
}

pid_t Launcher::startProcess() {
	setupEnvironment();
	
	pid_t childPid = createProcess();
	
	
	pid_t monitorPid = fork();
	if(monitorPid == -1) {
		std::cout << "Could not fork . . ." << std::endl;
		exit(1);
	}
	return monitorPid;
}

void Launcher::waitForChild(pid_t childPid) {
	std::cout << "In original process, waiting on process . . ." << std::endl;
	siginfo_t sinfo;
	waitid(P_PID, childPid, &sinfo, WEXITED);
	if(sinfo.si_code == CLD_DUMPED || sinfo.si_code == CLD_KILLED)
		Coordinator::instance()->setReturnValue(128 + sinfo.si_status);
	else Coordinator::instance()->setReturnValue(sinfo.si_status);
	
	std::cout << "Main Process: monitored process has terminated." << std::endl;
}

pid_t Launcher::createProcess() {
	pid_t childPid = fork();
	if(childPid == -1) {
		std::cout << "Could not fork . . ." << std::endl;
		exit(1);
	}
	else if(childPid == 0) {
		close(m_controllerFds[0]);
		execvp(m_argv[0], m_argv);
		std::cout << m_argv[0] << ": " << strerror(errno) << std::endl;
		exit(0);
	}
	
	close(m_controllerFds[1]);
	
	return childPid;
}

void Launcher::setupEnvironment() {
	pipe(m_controllerFds);
	
	setenv("AC___conductorFd", (Common::StreamAsString() << m_controllerFds[1]).operator std::string().c_str(), 1);
	
	std::vector<std::string> modules;
	
	Coordinator::instance()->vault()->get("::modules", modules);
	
	std::string preload;
	
	if(getenv("LD_PRELOAD")) {
		preload = getenv("LD_PRELOAD");
		preload += ":";
	}
	
	for(std::vector<std::string>::iterator i = modules.begin(); i != modules.end(); ++i) {
		std::string moduleRoot = Coordinator::instance()->vault()->get(*i + ":root");
		std::string collectorPath = Coordinator::instance()->vault()->get(*i + ":collectorPath");
		if(collectorPath.length()) {
			if(preload.length()) {
				preload += ":";
			}
			preload += moduleRoot + collectorPath;
			
			std::vector<Config::Vault::KeyPair> configItems;
			Coordinator::instance()->vault()->match(*i + ":*", configItems);
			
			for(std::vector<Config::Vault::KeyPair>::iterator i = configItems.begin(); i != configItems.end(); ++i) {
				std::string envName = "AC_" + i->first;
				for(std::string::size_type s = 0; s < envName.size(); s ++) {
					if(envName[s] == ':') envName[s] = '_';
				}
				setenv(envName.c_str(), i->second.c_str(), 1);
			}
		}
	}

	setenv("LD_PRELOAD", preload.c_str(), 1);
}

} // namespace Program
} // namespace Monitor
