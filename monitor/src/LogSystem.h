#ifndef LogSystem_H
#define LogSystem_H

#include <string>

#include "DataTypes.h"

namespace Program {
class Analyzer;
} // namespace Program

class LogSystem {
public:
	static void logModuleMessage(uint16_t moduleID, std::string message);
	static void logConfigurationMessage(std::string message);
	static void logAnalyzerMessage(Program::Analyzer *analyzer, std::string message);
	static void logProgramMessage(std::string filename, std::string message);
	static void logNetworkMessage(std::string message);
	static void logReaderMessage(std::string message);
};

#endif
