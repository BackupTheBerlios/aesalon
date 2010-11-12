/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/Parser.cpp

*/

#include <iostream>  // for debugging
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <sys/stat.h>
#include <stdlib.h>

#include "config/Parser.h"
#include "common/ParsingException.h"
#include "common/StreamAsString.h"
#include "config/ConcreteVault.h"

namespace Monitor {
namespace Config {

void Parser::parse(ConcreteVault *vault, const std::string &configFile) {
	std::string currentModule;
	
	std::vector<Vault::KeyPair> paths;
	vault->match("PATH", paths);
	
	openFile(findFile(configFile, paths));
	
	if(!m_stream->is_open()) return;
	
	for(;;) {
		TokenType tokenType;
		std::string token = nextToken(tokenType);
		if(tokenType == END_OF_FILE) break;
		
		//std::cout << '"' << token << '"' << " of type " << nameOf(tokenType) << std::endl;
		
		if(tokenType == WORD && token == "include") {
			Parser().parse(vault, expectNextToken(QUOTED_WORD));
			expectNextSymbol(";");
		}
		else if(tokenType == WORD && token == "module") {
			currentModule = expectNextToken(WORD);
			expectNextSymbol("{");
		}
		else if(tokenType == SYMBOL && token == "}") {
			if(currentModule != "") {
				currentModule = "";
			}
			else throw Common::ParsingException("Extra \"}\"");
		}
		else if(tokenType == WORD && token == "use") {
			/*std::cout << "Parser: Using module \"" << expectNextToken(WORD) << "\"\n";*/
			std::string moduleName = expectNextToken(WORD);
			
			Parser().parse(vault, moduleName + "/module.conf");
			paths.clear();
			vault->match("PATH", paths);
			findFile(vault->get(moduleName + ":collectorPath"), paths);
			
			expectNextSymbol(";");
		}
		else if(tokenType == WORD) {
			std::string op = expectNextToken(SYMBOL);
			
			if(op == "=") vault->clear(token);
			
			do {
				TokenType nextType;
				std::string next = nextToken(nextType);
				
				//if(nextType == SYMBOL && next == ";") break;
				
				if(nextType == WORD || nextType == QUOTED_WORD) {
					if(currentModule != "") vault->set(
						Common::StreamAsString() << currentModule << ":" << token, next);
					
					else vault->set(token, next);
					
					std::cout << "Set \"" << currentModule << "::" << token << "\" to \""
						<< next << "\" with operator " << op << std::endl;
				}
				else throw Common::ParsingException("Invalid RHS");
				
				std::string sym = expectNextToken(SYMBOL);
				if(sym == ";") break;
				else if(sym == ",") {}
				else {
					throw Common::ParsingException(Common::StreamAsString()
						<< "Expected \",\" or \";\", got \"" << sym << "\"");
				}
			} while(true);
		}
		else {
			throw Common::ParsingException(Common::StreamAsString()
				<< "Syntax error at token \"" << token << "\"");
		}
		//std::cout << "\"" << token << "\"\n";
	}
	
	closeFile();
}

std::string Parser::findFile(const std::string &filename, std::vector<Vault::KeyPair> &paths) {
	if(filename[0] == '/') return filename;
	
	if(filename[0] == '~') {
		char *homeDirectory = getenv("HOME");
		if(homeDirectory == NULL) return filename;
		std::string path = filename;
		path.replace(0, 1, homeDirectory);
		return path;
	}
	
	for(std::vector<Vault::KeyPair>::const_iterator i = paths.begin(); i != paths.end(); ++i) {
		std::string fullPath = i->second + "/";
		fullPath += filename;
		struct stat possibleStat;
		if(stat(fullPath.c_str(), &possibleStat) == 0) return fullPath;
	}
	
	return filename;
}

void Parser::openFile(const std::string &configFile) {
	m_stream = new std::ifstream(configFile.c_str());
}

std::string Parser::nextToken(TokenType &type) {
	std::ifstream &stream = *m_stream;
	
	skipWhitespace();
	
	if(stream.eof()) {
		type = END_OF_FILE;
		return "";
	}
	
	if(stream.peek() == '#') {
		std::string line;
		std::getline(stream, line);
		return nextToken(type);
	}
	else if(stream.peek() == '"') {
		stream.get();  // skip "
		
		std::string word;
		while(stream.peek() != '"') {
			word += stream.get();
		}
		
		stream.get();  // skip "
		
		type = QUOTED_WORD;
	    return word;
	}
	else if(std::isalnum(stream.peek())) {
		std::string word;
		while(std::isalnum(stream.peek())) {
			word += stream.get();
		}
		
		type = WORD;
	    return word;
	}
	else if(stream.peek() == '+') {
		stream.get();  // skip '+'
		if(stream.peek() == '=') {
			stream.get();  // skip '='
			type = SYMBOL;
			return "+=";
		}
		
		std::string rest;
		stream >> rest;
		
		throw Common::ParsingException(Common::StreamAsString()
			<< "Unrecognized token: \"+" << rest << "\"");
	}
	else {
		std::string token;
		token += stream.get();
		
		type = SYMBOL;
		return token;
	}
}

std::string Parser::expectNextToken(TokenType expected) {
	TokenType actual;
	std::string token = nextToken(actual);
	
	if(actual != expected) {
		throw Common::ParsingException(Common::StreamAsString()
			<< "Expected " << nameOf(expected) << " token, found " << nameOf(actual)
			<< " token: \"" << token << "\"");
	}
	
	return token;
}

void Parser::expectNextSymbol(const std::string &symbol) {
	std::string s = expectNextToken(SYMBOL);
	if(symbol != s) {
		throw Common::ParsingException(Common::StreamAsString()
			<< "Expected token \"" << symbol << "\", got \"" << s << "\"");
	}
}

void Parser::skipWhitespace() {
	std::ifstream &stream = *m_stream;
	
	while(!stream.fail() && std::isspace(stream.peek())) {
		stream.get();
	}
}

void Parser::closeFile() {
	delete m_stream;
	m_stream = NULL;
}

const char *Parser::nameOf(TokenType type) const {
	static const char *name[] = {
		"WORD",
		"QUOTED_WORD",
		"SYMBOL",
		"END_OF_FILE"
	};
	
	return name[type];
}

} // namespace Config
} // namespace Monitor
