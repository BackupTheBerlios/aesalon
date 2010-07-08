#include <iostream>

#include "LogSystem.h"

void LogSystem::logModuleMessage(uint16_t moduleID, std::string message) {
	std::cout << "[" << moduleID << "] " << message << std::endl;
}

void LogSystem::logConfigurationMessage(std::string message) {
	std::cout << "[configuration] " << message << std::endl;
}
