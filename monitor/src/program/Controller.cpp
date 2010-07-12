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
#include "module/Reader.h"

namespace Program {

Controller::Controller(pid_t childPid) : m_childPid(childPid) {
	m_analyzer = Initializer::singleton()->launcher()->analyzer();
	m_sharedMemory = Initializer::singleton()->launcher()->sharedMemory();
}

Controller::~Controller() {
	m_sharedMemory->setFinished();
	pthread_join(m_readerThread, NULL);
}

void Controller::run() {
	/* Wait for the SIGTRAP. */
	waitForSigTrap();

	Symbol *mainSymbol = m_analyzer->symbol("main");
	if(mainSymbol == NULL) {
		LogSystem::logProgramMessage(m_analyzer->filename(), "Cannot resolve main(). Program is probably stripped.");
		return;
	}
	Address mainAddress = mainSymbol->address();
	uint8_t originalContent = readData(mainAddress) & 0xff;
	writeData(mainAddress, 0xcc);
	
	continueExecution();
	
	/* Wait for the second SIGTRAP . . .*/
	waitForSigTrap();
	
	setIp(getIp() - 1);
	writeData(mainAddress, originalContent);
	
	/* Tell the collector libraries that main() has been reached. */
	m_sharedMemory->setMainReached();
	
	/* Create the reading thread(s). */
	createThreads();
	
	/* Continue execution of the monitored program. */
	continueExecution();
	
	/* Detach from the process. */
	detachFromProcess();
	
	int status;
	/* Now, wait for the termination signal. */
	waitForSignal(&status);
	std::cout << "Received termination signal . . ." << std::endl;
	std::cout << "\tSignal number: " << signalFromStatus(status) << std::endl;
}

void Controller::waitForSigTrap() {
	int status;
	do {
		waitForSignal(&status);
	} while(!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP);
}

int Controller::waitForSignal(int *status) const {
	return waitpid(m_childPid, status, 0);
}

int Controller::checkForSignal(int *status) const {
	return waitpid(m_childPid, status, WNOHANG);
}

int Controller::signalFromStatus(int status) {
	if(WIFSTOPPED(status)) return WSTOPSIG(status);
	else if(WIFSIGNALED(status)) return WTERMSIG(status);
	else if(WIFEXITED(status)) return -1;
	/* Unknown signal, ignore. */
	return 0;
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
		return 0;
	}
	return value;
}

void Controller::continueExecution(int signal) {
	ptrace(PTRACE_CONT, m_childPid, NULL, signal);
}

void Controller::detachFromProcess() {
	ptrace(PTRACE_DETACH, m_childPid);
}

static void *PacketReaderThread(void *unused) {
	SharedMemory *shm = Initializer::singleton()->launcher()->sharedMemory();
	Module::Reader *reader = new Module::Reader();
	DataPacket *packet;
	while((packet = shm->readPacket())) reader->processPacket(packet);
	return NULL;
}

void Controller::createThreads() {
	pthread_create(&m_readerThread, NULL, PacketReaderThread, NULL);
}

} // namespace Program
