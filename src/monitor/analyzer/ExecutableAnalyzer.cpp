/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/analyzer/ExecutableAnalyzer.cpp

*/

#include "analyzer/ExecutableAnalyzer.h"
#include "analyzer/ElfAnalyzer.h"

namespace Monitor {
namespace Analyzer {

ExecutableAnalyzer::ExecutableAnalyzer(std::string filename) : m_filename(filename) {
	
}

ExecutableAnalyzer::~ExecutableAnalyzer() {
	
}

Config::Vault *ExecutableAnalyzer::analyzeExecutable(std::string filename) {
	ElfAnalyzer elfAnalyzer(filename);
	if(elfAnalyzer.analyzerVault()) return elfAnalyzer.analyzerVault();
	return NULL;
}

} // namespace Analyzer
} // namespace Monitor
