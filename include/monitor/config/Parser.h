/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/Parser.h

*/

#ifndef AesalonMonitor_Config_Parser_H
#define AesalonMonitor_Config_Parser_H

#include <iosfwd>

#include "Vault.h"

namespace Monitor {
namespace Config {

class ConcreteVault;

class Parser {
private:
	std::ifstream *m_stream;
	
	enum TokenType {
		WORD,
		QUOTED_WORD,
		SYMBOL,
		END_OF_FILE
	};
public:
	void parse(ConcreteVault *vault, const std::string &configFile);
private:
	void openFile(const std::string &configFile);
	std::string nextToken(TokenType &type);
	std::string expectNextToken(TokenType expected);
	void expectNextSymbol(const std::string &symbol);
	void skipWhitespace();
	void closeFile();
	
	const char *nameOf(TokenType type) const;
};

} // namespace Config
} // namespace Monitor

#endif
