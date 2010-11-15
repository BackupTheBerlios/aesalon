/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/Coordinator.cpp

*/

#include <iostream>
#include <limits.h>
#include <stdlib.h>

#include "Coordinator.h"
#include "config/ArgumentParser.h"
#include "config/Parser.h"
#include "common/Config.h"
#include "program/Launcher.h"
#include "module/Module.h"
#include "config/ConcreteVault.h"
#include "common/PathSanitizer.h"
#include "program/Conductor.h"

namespace Monitor {

Coordinator *Coordinator::m_instance = NULL;

Coordinator::Coordinator(char **argv) : m_argv(argv) {
	m_instance = this;
}

Coordinator::~Coordinator() {
	
}

void Coordinator::run() {
	parseConfigs();
	
	if(m_vault->get("::list-attributes") == "true") {
		std::cout << "Listing all configuration keys and values." << std::endl;
		std::cout << "Please note that many of these are auto-generated." << std::endl;
		std::cout << "================" << std::endl;
		std::vector<Config::Vault::KeyPair> list;
		m_vault->match("*", list);
		for(std::vector<Config::Vault::KeyPair>::iterator i = list.begin(); i != list.end(); ++i) {
			// NOTE: left out for debugging purposes
			//if(i->first[0] == ':' && i->first[1] == ':') continue;
			
			std::cout << "    * \"" << i->first << "\" ==> \"" << i->second << "\"\n";
		}
	}
	else if(m_argv[m_argcOffset] == NULL || m_vault->get("::help") == "true") {
		usage(true);
		return;
	}
	
	Program::Launcher *launcher = new Program::Launcher(&m_argv[m_argcOffset]);
	
	if(launcher->startProcess() == 0) {
		Program::Conductor *conductor = new Program::Conductor(launcher->readFd());
		
		conductor->monitor();
	}
	
	delete launcher;
}

void Coordinator::parseConfigs() {
	Config::Parser parser;
	
	Config::ConcreteVault *vault = new Config::ConcreteVault();
	
	parser.parse(vault, Common::PathSanitizer::sanitize(AesalonGlobalConfig));
	parser.parse(vault, Common::PathSanitizer::sanitize(AesalonUserConfig));
	parser.parse(vault, Common::PathSanitizer::sanitize(AesalonLocalConfig));
	
	Config::ArgumentParser argParser;
	m_argcOffset = argParser.parse(vault, m_argv);
	
	m_vault = vault;
}

void Coordinator::usage(bool displayHelp) {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2010 Aesalon Development Team." << std::endl;
	std::cout << "Aesalon is released under the GNU General Public License, version 3." << std::endl;
	
	if(!displayHelp) return;
	std::cout << std::endl;
	std::cout << "Usage: " << m_argv[0] << " [options] [--] executable [arguments]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t--help\n\t\tDisplays this usage message." << std::endl;
	std::cout << "\t--version\n\t\tDisplays version information." << std::endl;
	std::cout << "\t--search <path>\n\t\tSearches <path> for modules." << std::endl;
	std::cout << "\t--use-module <module>\n\t\tPrepares <module> for loading." << std::endl;
	std::cout << "\t--set <attribute>[=value]\n\t\tSets a module attribute." << std::endl;
	std::cout << "\t--list-attributes\n\t\tLists all the available attributes." << std::endl;
}

} // namespace Monitor
