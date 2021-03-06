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
#include "config/GlobalVault.h"
#include "monitor/ZoneReader.h"
#include "util/MessageSystem.h"

namespace Monitor {

Launcher::Launcher(char **argv) : m_argv(argv) {

}

Launcher::~Launcher() {
	if(m_shmReader) delete m_shmReader;
}

void Launcher::launch() {
	forkTarget();
	
	ZoneReader *reader = new ZoneReader(m_shmReader);
	reader->run();
}

void Launcher::forkTarget() {
	setupEnvironment();
	
	m_shmReader = new SHMReader();
	
	m_targetPid = fork();
	if(m_targetPid == -1) {
		Message(Fatal, "Could not fork target " << m_argv[0] << ": " << std::strerror(errno));
		std::cout << "Could not fork . . ." << std::endl;
		exit(1);
	}
	else if(m_targetPid == 0) {
		execvp(m_argv[0], m_argv);
		Message(Fatal, "Could not launch " << m_argv[0] << ": " << std::strerror(errno));
	}
}

void Launcher::setupEnvironment() {
	Config::Vault *vault = Config::GlobalVault::instance();
	setenv("AesalonSHMName", (Util::StreamAsString() << "/Aesalon-" << getpid()).operator std::string().c_str(), 1);
	
	std::vector<std::string> modules;
	
	vault->get("::modules", modules);
	
	std::string preload;
	
	if(getenv("LD_PRELOAD")) {
		Message(Warning, "Replacing current LD_PRELOAD.");
	}
	
	for(std::vector<std::string>::iterator i = modules.begin(); i != modules.end(); ++i) {
		std::string moduleRoot = vault->get(*i + ":root");
		std::string collectorPath = vault->get(*i + ":collectorPath");
		if(collectorPath.length()) {
			if(preload.length()) {
				preload += ":";
			}
			preload += moduleRoot + collectorPath;
		}
	}
	
	setenv("LD_PRELOAD", preload.c_str(), 1);
}

} // namespace Monitor
