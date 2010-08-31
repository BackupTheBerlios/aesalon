#include <iostream>

#include "LogSystem.h"
#include "program/Analyzer.h"

std::ostream *LogSystem::m_stream = &std::cerr;

void LogSystem::logModuleMessage(uint16_t moduleID, std::string message) {
	(*m_stream) << "[" << moduleID << "] " << message << std::endl;
}

void LogSystem::logConfigurationMessage(std::string message) {
	(*m_stream) << "[configuration] " << message << std::endl;
}

void LogSystem::logAnalyzerMessage(Program::Analyzer *analyzer, std::string message) {
	(*m_stream) << "[" << analyzer->filename() << "] " << message << std::endl;
}

void LogSystem::logProgramMessage(std::string filename, std::string message) {
	(*m_stream) << "[" << filename << "] " << message << std::endl;
}

void LogSystem::logNetworkMessage(std::string message) {
	(*m_stream) << "[network] " << message << std::endl;
}

void LogSystem::logReaderMessage(std::string message) {
	(*m_stream) << "[reader] " << message << std::endl;
}
