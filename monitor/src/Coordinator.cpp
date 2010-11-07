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
#include "config/FileParser.h"
#include "common/Config.h"
#include "program/Launcher.h"

namespace Monitor {

Coordinator *Coordinator::m_instance = NULL;

Coordinator::Coordinator(char **argv) : m_argv(argv) {
	m_instance = this;
}

Coordinator::~Coordinator() {
	
}

void Coordinator::run() {
	parseConfigs();
	if(m_argv[m_argumentEndpoint] == NULL || m_store->item("help")->boolValue()) {
		usage();
		return;
	}
	
	Program::Launcher *launcher = new Program::Launcher(m_store, &m_argv[m_argumentEndpoint]);
}

void Coordinator::parseConfigs() {
	m_store = new Config::Store();
	/* Perform first pass through configuration . . . */
	Config::FileParser fp(m_store);
	fp.parse(AesalonGlobalConfig);
	fp.parse(AesalonUserConfig);
	
	Config::ArgumentParser ap(m_store, m_argv);
	m_argumentEndpoint = ap.parse();
	
	
}

void Coordinator::usage() {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2010 Aesalon Development Team." << std::endl;
	std::cout << "Aesalon is released under the GNU General Public License, version 3." << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: " << m_argv[0] << " [options] [--] executable [arguments]" << std::endl;
	std::cout << "Options:" << std::endl;
}

} // namespace Monitor
