#include <iostream>

#include "LogSystem.h"
#include "program/Analyzer.h"

void LogSystem::logModuleMessage(uint16_t moduleID, std::string message) {
	std::cout << "[" << moduleID << "] " << message << std::endl;
}

void LogSystem::logConfigurationMessage(std::string message) {
	std::cout << "[configuration] " << message << std::endl;
}

void LogSystem::logAnalyzerMessage(Program::Analyzer *analyzer, std::string message) {
	std::cout << "[" << analyzer->filename() << "] " << message << std::endl;
}

void LogSystem::logProgramMessage(std::string filename, std::string message) {
	std::cout << "[" << filename << "] " << message << std::endl;
}

void LogSystem::logNetworkMessage(std::string message) {
	std::cout << "[network] " << message << std::endl;
}

void LogSystem::logReaderMessage(std::string message) {
	std::cout << "[reader] " << message << std::endl;
}
