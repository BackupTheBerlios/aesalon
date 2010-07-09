#include <sys/wait.h>

#include "Controller.h"

namespace Program {

Controller::Controller(pid_t childPid) : m_childPid(childPid) {
	
}

Controller::~Controller() {
	
}

int Controller::waitForSignal(int *status) const {
	return waitpid(m_childPid, status, 0);
}

int Controller::checkForSignal(int *status) const {
	return waitpid(m_childPid, status, WNOHANG);
}


} // namespace Program
