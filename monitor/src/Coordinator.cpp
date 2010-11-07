/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/Coordinator.cpp

*/

#include <iostream>

#include "Coordinator.h"
#include "config/ArgumentParser.h"
#include "config/Parser.h"
#include "common/Config.h"
#include "program/Launcher.h"
#include "module/Module.h"

#include "config/ConcreteVault.h"

namespace Monitor {

Coordinator *Coordinator::m_instance = NULL;

Coordinator::Coordinator(char **argv) : m_argv(argv) {
	m_instance = this;
}

Coordinator::~Coordinator() {
	
}

void Coordinator::run() {
	parseConfigs();
	/*if(m_argv[m_argumentEndpoint] == NULL || m_store->item("help")->boolValue()
		|| m_store->item("version")->boolValue()) {
		
		usage(!m_store->item("version")->boolValue());
		return;
	}
	
	Program::Launcher *launcher = new Program::Launcher(m_store, &m_argv[m_argumentEndpoint]);*/
}

void Coordinator::parseConfigs() {
	Config::Parser parser;
	
	Config::Vault *vault = new Config::ConcreteVault();
	parser.parse(vault, /*AesalonLocalConfig*/ ".aesalon.conf");
}

std::string Coordinator::moduleRoot(const std::string &moduleName) {
	std::string searchPath;
	return "";
}

void Coordinator::usage(bool displayHelp) {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2010 Aesalon Development Team." << std::endl;
	std::cout << "Aesalon is released under the GNU General Public License, version 3." << std::endl;
	
	if(!displayHelp) return;
	std::cout << std::endl;
	std::cout << "Usage: " << m_argv[0] << " [options] [--] executable [arguments]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t--help: Displays this usage message." << std::endl;
	std::cout << "\t--version: Displays version information." << std::endl;
}

} // namespace Monitor
