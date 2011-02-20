/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Coordinator.cpp
*/

#include <iostream>

#include "Config.h"

#include "monitor/Coordinator.h"
#include "monitor/ArgumentParser.h"
#include "monitor/Launcher.h"
#include "config/Parser.h"
#include "util/PathSanitizer.h"
#include "util/MessageSystem.h"

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
		Message(Log, "Listing all configuration keys and values.");
		Message(Log, "Some auto-generated values may have non-canonical values.");
		std::vector<Config::Vault::KeyPair> list;
		m_vault->match("*", list);
		for(std::vector<Config::Vault::KeyPair>::iterator i = list.begin(); i != list.end(); ++i) {
			if(i->first[0] == ':' && i->first[1] == ':') continue;
			
			Message(Log, "    * \"" << i->first << "\" ==> \"" << i->second << "\"");
		}
	}
	else if(m_vault->get("::version") == "true") {
		usage(false);
	}
	else if(m_argv[m_argcOffset] == NULL || m_vault->get("::help") == "true") {
		usage(true);
	}
	else {
		Launcher launcher(m_argv + m_argcOffset);
		
		launcher.launch();
	}
}

void Coordinator::parseConfigs() {
	Config::Parser parser;
	m_vault = new Config::Vault();
	
	parser.parse(m_vault, Util::PathSanitizer::sanitize(AesalonMonitorGlobalConfig));
	parser.parse(m_vault, Util::PathSanitizer::sanitize(AesalonMonitorUserConfig));
	parser.parse(m_vault, Util::PathSanitizer::sanitize(AesalonMonitorLocalConfig));
	
	m_argcOffset = ArgumentParser::parse(m_vault, m_argv);
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