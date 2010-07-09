#include <cstring>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/stat.h>

#include "Launcher.h"
#include "Initializer.h"
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
	
	m_argv = new char *[programArguments.size() + 2];
	
	for(int i = 0; i < (int)programArguments.size(); i ++) {
		m_argv[i] = new char[programArguments[i].length() + 1];
		strcpy(m_argv[i], programArguments[i].c_str());
	}
	m_argv[programArguments.size() - 1] = 0;
}

void Launcher::startProcess() {
	m_childPid = fork();
	if(m_childPid == 0) {
		setenv("LD_PRELOAD", preload().c_str(), 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execv(m_argv[0], m_argv);
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
	
	std::string preload = getenv("LD_PRELOAD");
	
	if(preload.length()) preload += ":";
	
	do {
		std::string moduleName = moduleList.substr(0, moduleList.find(","));
		moduleList.erase(0, moduleList.find(",")+1);
		
		for(std::vector<std::string>::iterator i = searchPaths.begin(); i != searchPaths.end(); i ++) {
			struct stat possibleStat;
			std::string possiblePath = Misc::StreamAsString() << *i << "/lib" << moduleName << ".so";
			if(stat(possiblePath.c_str(), &possibleStat) == 0) {
				preload += possiblePath;
				preload += ":";
				break;
			}
		}
	} while(pathList.find(",") != std::string::npos);
	
	preload.erase(preload.length()-1, 1);
	return preload;
}


} // namespace Program
