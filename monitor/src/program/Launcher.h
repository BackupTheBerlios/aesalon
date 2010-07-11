#ifndef Launcher_H
#define Launcher_H

#include <sys/types.h>

#include "SharedMemory.h"

namespace Program {

class Launcher {
public:
	Launcher();
	~Launcher();
private:
	char **m_argv;
	SharedMemory *m_sharedMemory;
	pid_t m_childPid;
public:
	SharedMemory *sharedMemory() const { return m_sharedMemory; }
	pid_t childPid() const { return m_childPid; }
	std::string filename() const { return m_argv[0]; }
private:
	void assembleArgv();
	void startProcess();
	std::string preload();
};

} // namespace Program

#endif
