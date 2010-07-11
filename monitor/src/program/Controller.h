#ifndef Controller_H
#define Controller_H

#include <sys/types.h>

#include "DataTypes.h"
#include "Analyzer.h"

namespace Program {

class Controller {
public:
	Controller(pid_t childPid);
	~Controller();
private:
	pid_t m_childPid;
	int m_returnCode;
	Analyzer *m_analyzer;
public:
	pid_t childPid() const { return m_childPid; }
	int returnCode() const { return m_returnCode; }
	
	void run();
protected:
	void waitForSigTrap();
	int waitForSignal(int *status) const;
	int checkForSignal(int *status) const;
	int signalFromStatus(int status);
	Address getIp() const;
	void setIp(Address value) const;
	void writeData(Address memoryAddress, uint8_t data) const;
	Address readData(Address memoryAddress) const;
	void continueExecution(int signal = 0);
};

} // namespace Program

#endif
