#include <cstring>
#include <sys/ptrace.h>

#include "Launcher.h"
#include "Initializer.h"

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
	m_argv[0] = new char[filename.length() + 1];
	strcpy(m_argv[0], filename.c_str());
	
	for(int i = 0; i < (int)programArguments.size(); i ++) {
		m_argv[i+1] = new char[programArguments[i].length() + 1];
		strcpy(m_argv[i+1], programArguments[i].c_str());
	}
	m_argv[programArguments.size()] = 0;
}

void Launcher::startProcess() {
	m_childPid = fork();
	if(m_childPid == 0) {
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execv(m_argv[0], m_argv);
	}
	m_sharedMemory = new SharedMemory(1);
}

} // namespace Program
