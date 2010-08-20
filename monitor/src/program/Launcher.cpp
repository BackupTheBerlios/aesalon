#include <cstring>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <iostream>
#include <errno.h>
#include <signal.h>
#include <stdio.h>

#include "Launcher.h"
#include "Initializer.h"
#include "misc/StreamAsString.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"
#include "ElfAnalyzer.h"
#include "misc/PathSanitizer.h"

namespace Program {

Launcher::Launcher() {
	m_sharedMemory = NULL;
	m_analyzer = NULL;
	m_controller = NULL;
}

Launcher::~Launcher() {
	if(m_controller) delete m_controller;
	if(m_analyzer) delete m_analyzer;
	if(m_sharedMemory) delete m_sharedMemory;
}

void Launcher::start() {
	assembleArgv();
	startProcess();
}

void Launcher::assembleArgv() {
	Misc::Configuration *config = Initializer::singleton()->configuration();
	
	std::string filename = config->filename();
	const std::vector<std::string> &programArguments = config->programArguments();
	
	m_argv = static_cast<char **>(malloc(sizeof(char *) * (programArguments.size() + 2)));
	
	for(int i = 0; i < (int)programArguments.size(); i ++) {
		m_argv[i] = static_cast<char *>(malloc(sizeof(char) * (programArguments[i].length() + 1)));
		strcpy(m_argv[i], programArguments[i].c_str());
	}
	m_argv[programArguments.size()] = 0;
}

void Launcher::startProcess() {
	m_childPid = fork();
	if(m_childPid == 0) {
		setenv("LD_PRELOAD", preload().c_str(), 1);
		char buffer[128];
		sprintf(buffer, "%i", Initializer::singleton()->configuration()->configItems()["shm-size"]->intValue());
		setenv("AC_ShmSize", buffer, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		
		setModuleEnvironment();
		
		execv(m_argv[0], m_argv);
		LogSystem::logProgramMessage(m_argv[0], Misc::StreamAsString() << "execv() failed: " << strerror(errno));
		exit(0);
	}
	m_sharedMemory = new SharedMemory(m_childPid);
	m_analyzer = new ElfAnalyzer(m_argv[0]);
	m_controller = new Controller(m_childPid);
	
	m_controller->run();
}

std::string Launcher::preload() {
	std::vector<std::string> searchPaths;
	std::string pathList = Initializer::singleton()->configuration()->configItems()["search-path"]->stringValue();
	
	do {
		searchPaths.push_back(pathList.substr(0, pathList.find(":")));
		pathList.erase(0, pathList.find(":")+1);
	} while(pathList.find(":") != std::string::npos);
	
	std::string moduleList = Initializer::singleton()->configuration()->configItems()["modules"]->stringValue();
	
	char *oldPreload = getenv("LD_PRELOAD");
	std::string preload;
	if(oldPreload) {
		preload = oldPreload;
		preload += ":";
	}
	
	do {
		std::string moduleName = moduleList.substr(0, moduleList.find(":"));
		moduleList.erase(0, moduleList.find(":")+1);
		
		std::string found = Misc::PathSanitizer::findFromPaths(Initializer::singleton()->configuration()
			->configItems()["collector"]->childValue(moduleName)->stringValue(),
			Initializer::singleton()->configuration()->configItems()["_module-path"]->
			childValue(moduleName)->stringValue());
		
		if(found.length()) {
			preload += found;
			preload += ':';
		}
	} while(pathList.find(":") != std::string::npos);
	
	if(preload.length()) {
		preload += Misc::PathSanitizer::findFromPaths("interface.so", pathList);
	}
	
	return preload;
}

void Launcher::setModuleEnvironment() {
	
}

} // namespace Program
