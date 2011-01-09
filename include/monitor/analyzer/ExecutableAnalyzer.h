/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/analyzer/ExecutableAnalyzer.h

*/

#ifndef AesalonMonitor_Analyzer_ExecutableAnalyzer_H
#define AesalonMonitor_Analyzer_ExecutableAnalyzer_H

#include "AbstractAnalyzer.h"

namespace Monitor {
namespace Analyzer {

class ExecutableAnalyzer : public AbstractAnalyzer {
protected:
	std::string m_filename;
public:
	ExecutableAnalyzer(std::string filename);
	virtual ~ExecutableAnalyzer();
	
	/** Config vault items are of the form SymbolName:SymbolAddress.
	*/
	virtual Config::Vault *analyzerVault() = 0;
};

} // namespace Analyzer
} // namespace Monitor

#endif
