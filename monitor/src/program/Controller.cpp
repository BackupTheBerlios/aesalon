#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <iostream>

#include "Controller.h"
#include "LogSystem.h"
#include "Initializer.h"

namespace Program {

Controller::Controller(pid_t childPid) : m_childPid(childPid) {
	m_analyzer = Initializer::singleton()->launcher()->analyzer();
}

Controller::~Controller() {
	
}

void Controller::run() {
	/* Wait for the SIGTRAP. */
	std::cout << "Waiting for exec() sigtrap . . . " << std::endl;
	waitForSigTrap();
	Symbol *mainSymbol = m_analyzer->symbol("main");
	if(mainSymbol == NULL) {
		LogSystem::logProgramMessage(m_analyzer->filename(), "Cannot resolve main(). Program is probably stripped.");
		return;
	}
	Address mainAddress = mainSymbol->address();
	uint8_t originalContent = readData(mainAddress) & 0xff;
	writeData(mainAddress, 0xcc);
	std::cout << "new data at " << mainAddress << " is " << int(readData(mainAddress) & 0xff) << std::endl;
	std::cout << "original data is " << int(originalContent & 0xff) << std::endl;
	continueExecution();
	std::cout << "Waiting for main() sigtrap . . . " << std::endl;
	/* Wait for the second SIGTRAP . . .*/
	waitForSigTrap();
	
	setIp(getIp() - 1);
	writeData(mainAddress, originalContent);
	
	continueExecution();
	
	/* Now, wait for the termination signal. */
	waitForSignal(NULL);
}

void Controller::waitForSigTrap() {
	int status;
	do {
		std::cout << "Waiting for signal . . .\n" << std::endl;
		waitForSignal(&status);
	} while(!WIFSTOPPED(status) || !WSTOPSIG(status) == SIGTRAP);
}

int Controller::waitForSignal(int *status) const {
	return waitpid(m_childPid, status, 0);
}

int Controller::checkForSignal(int *status) const {
	return waitpid(m_childPid, status, WNOHANG);
}

Address Controller::getIp() const {
	struct user_regs_struct registers;
	ptrace(PTRACE_GETREGS, m_childPid, NULL, &registers);
	return registers.rip;
}

void Controller::setIp(Address value) const {
	struct user_regs_struct registers;
	ptrace(PTRACE_GETREGS, m_childPid, NULL, &registers);
	registers.rip = value;
	ptrace(PTRACE_SETREGS, m_childPid, NULL, &registers);
}

void Controller::writeData(Address memoryAddress, uint8_t data) const {
	Address offset = memoryAddress & 0x07;
	Address value = readData(memoryAddress & ~0x07);
    
	value &= ~(Address(0xff) << (offset * CHAR_BIT));
	value |= (Address(data) << (offset * CHAR_BIT));
	ptrace(PTRACE_POKEDATA, m_childPid, memoryAddress & ~0x07, value);
}

Address Controller::readData(Address memoryAddress) const {
	Address value;
	value = ptrace(PTRACE_PEEKDATA, m_childPid, memoryAddress);
	if(value == Address(-1) && errno != 0) {
		std::cout << "Error: " << strerror(errno) << std::endl;
	}
	std::cout << std::hex << "value: " << value << std::endl;
	return value;
}

void Controller::continueExecution() {
	ptrace(PTRACE_CONT, m_childPid);
}

} // namespace Program
