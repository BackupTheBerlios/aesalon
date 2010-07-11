#ifndef Launcher_H
#define Launcher_H

#include <sys/types.h>

#include "SharedMemory.h"
#include "Analyzer.h"
#include "Controller.h"

namespace Program {

class Launcher {
public:
	Launcher();
	~Launcher();
private:
	char **m_argv;
	SharedMemory *m_sharedMemory;
	Analyzer *m_analyzer;
	Controller *m_controller;
	pid_t m_childPid;
public:
	SharedMemory *sharedMemory() const { return m_sharedMemory; }
	Analyzer *analyzer() const { return m_analyzer; }
	Controller *controller() const { return m_controller; }
	
	pid_t childPid() const { return m_childPid; }
	std::string filename() const { return m_argv[0]; }
	int returnCode() const { return m_controller->returnCode(); }
	
	void start();
private:
	void assembleArgv();
	void startProcess();
	std::string preload();
};

} // namespace Program

#endif
