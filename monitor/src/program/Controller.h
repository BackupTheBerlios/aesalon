#ifndef Controller_H
#define Controller_H

#include <sys/types.h>

namespace Program {

class Controller {
public:
	Controller(pid_t childPid);
	~Controller();
private:
	pid_t m_childPid;
public:
	pid_t childPid() const { return m_childPid; }
	
	int waitForSignal(int *status) const;
	int checkForSignal(int *status) const;
};

} // namespace Program

#endif
