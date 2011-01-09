/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/analyzer/AbstractAnalyzer.h

*/

#ifndef AesalonMonitor_Analyzer_AbstractAnalyzer_H
#define AesalonMonitor_Analyzer_AbstractAnalyzer_H

#include "config/Vault.h"

namespace Monitor {
namespace Analyzer {

/** Abstract base class that represents an Analyzer.
*/
class AbstractAnalyzer {
public:
	virtual ~AbstractAnalyzer() {}
	/** Returns the information found by the Analyzer as a configuration vault.
	*/
	virtual Config::Vault *analyzerVault() = 0;
};

} // namespace Analyzer
} // namespace Monitor

#endif
