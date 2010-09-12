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
	if(m_configuration->traverse("help")->asBool()) {
		usage();
		return 0;
	}
	else if(m_configuration->launchArguments().size() == 0) {
		LogSystem::logConfigurationMessage("No filename specified. Displaying usage information.");
		usage();
		return 0;
	}
	
	m_socketManager = new Network::TcpManager(m_configuration->traverse("tcpPort")->asInt());
	m_socketManager->waitForConnections(m_configuration->traverse("networkWait")->asInt());
	
	m_launcher = new Program::Launcher();
	
	m_launcher->start();
	
	return m_launcher->returnCode();
}

void Initializer::usage() {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2009-2010, strange." << std::endl;
	std::cout << "This program is released under the GNU GPLv3. " << std::endl;
	std::cout << "\tFor more legal information, see the included file LICENSE." << std::endl;
	std::cout << "usage: " << m_argv[0] << " [options] [--] filename [arguments]" << std::endl;
	
	std::cout << "\nOptions:" << std::endl;
	std::cout << "\t--help: displays this help message." << std::endl;
	std::cout << "\t--networkWait: amount of network clients to wait for before executing." << std::endl;
	std::cout << "\t--tcpPort: TCP port to listen on." << std::endl;
	std::cout << "\t--logFile: optional log file to write collected data to." << std::endl;
	std::cout << "\t--shmSize: size of the SHM used to communicate." << std::endl;
	
	std::cout << "\nEnvironment variables:" << std::endl;
	std::cout << "\tAesalonSearchPath: a colon-separated path to search for modules." << std::endl;
}
