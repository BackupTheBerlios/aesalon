/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/config/Parser.h
*/

#ifndef AesalonConfig_Parser_H
#define AesalonConfig_Parser_H

#include <fstream>

#include "Vault.h"

namespace Config {

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
	void parse(Vault *vault, const std::string &configFile);
	void parseDirectory(Vault *vault, const std::string &directory);
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

#endif

