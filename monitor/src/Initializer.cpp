#include <iostream>

#include "Initializer.h"
#include "LogSystem.h"
#include "program/ElfAnalyzer.h"
#include "network/TcpManager.h"

Initializer *Initializer::m_singleton = 0;

Initializer::Initializer(char *argv[]) : m_argv(argv) {
	m_singleton = this;
	m_configuration = new Misc::Configuration(m_argv);
	m_moduleMapper = new Module::ModuleMapper();
	m_socketManager = NULL;
	m_launcher = NULL;
	m_socketManager = NULL;
}

Initializer::~Initializer() {
	if(m_launcher) delete m_launcher;
	if(m_socketManager) delete m_socketManager;
	if(m_moduleMapper) delete m_moduleMapper;
	if(m_configuration) delete m_configuration;
}

int Initializer::run() {
	/* TODO: reinsert */
	if(m_configuration->traverse("global.help")->asBool() || m_configuration->launchArguments().size() == 0) {
		if(m_configuration->launchArguments().size() == 0)
			LogSystem::logConfigurationMessage("No filename specified. Displaying usage information.");
		usage();
		return 0;
	}
	
	/* TODO: reinsert */
	/*m_socketManager = new Network::TcpManager(m_configuration->configItems()["tcp-port"]->intValue());
	m_socketManager->waitForConnections(m_configuration->configItems()["network-wait"]->intValue());*/
	
	m_launcher = new Program::Launcher();
	
	m_launcher->start();
	
	return m_launcher->returnCode();
}

void Initializer::usage() {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2010 by strange." << std::endl;
	std::cout << "This program is released under the GNU GPLv3. For more legal information, see the LICENSE file." << std::endl;
	std::cout << "usage: " << m_argv[0] << " [options] [--] filename [arguments]" << std::endl;
	std::cout << "Options:" << std::endl;
}
