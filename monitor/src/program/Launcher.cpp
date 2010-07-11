#include <cstring>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <iostream>
#include <errno.h>
#include <signal.h>

#include "Launcher.h"
#include "Initializer.h"
#include "misc/StreamAsString.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"

namespace Program {

Launcher::Launcher() {
	assembleArgv();
	startProcess();
}

Launcher::~Launcher() {
	
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
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execv(m_argv[0], m_argv);
		LogSystem::logProgramMessage(m_argv[0], Misc::StreamAsString() << "execv() failed: " << strerror(errno));
		exit(0);
	}
	m_sharedMemory = new SharedMemory(m_childPid);
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
		std::string moduleName = moduleList.substr(0, moduleList.find(","));
		moduleList.erase(0, moduleList.find(",")+1);
		
		for(std::vector<std::string>::iterator i = searchPaths.begin(); i != searchPaths.end(); i ++) {
			struct stat possibleStat;
			std::string possiblePath = Misc::StreamAsString() << *i << "/lib" << moduleName << "Collector.so";
			if(stat(possiblePath.c_str(), &possibleStat) == 0) {
				preload += possiblePath;
				preload += ":";
				break;
			}
			else {
				std::cout << "Launcher::preload(): Couldn't open \"" << possiblePath << "\"; trying next search path.\n";
			}
		}
	} while(pathList.find(",") != std::string::npos);
	
	if(preload.length()) {
		/* TODO: make this path non hard-coded. */
		preload += "modules/build/libcollectorInterface.so";
	}
	return preload;
}


} // namespace Program
