/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/config/Parser.cpp
*/

#include <cstring>
#include <cstdlib>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>

#include "config/Parser.h"
#include "util/StreamAsString.h"
#include "config/ParsingException.h"
#include "Config.h"

namespace Config {

void Parser::parse(Vault *vault, const std::string &configFile) {
	std::string currentModule;
	
	openFile(configFile);
	
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
		else if(tokenType == WORD && token == "search") {
			char *dirpath = strdup(configFile.c_str());
			dirpath = dirname(dirpath);
			
			std::string dirname = expectNextToken(QUOTED_WORD);
			
			if(dirname[0] != '/' && dirname[0] != '~') {
				dirname = Util::StreamAsString() << dirpath << "/" << dirname;
			}
			
			Parser().parseDirectory(vault, dirname);
			
			expectNextSymbol(";");
			std::free(dirpath);
		}
		else if(tokenType == SYMBOL && token == "}") {
			if(currentModule != "") {
				currentModule = "";
			}
			else throw ParsingException("Extra \"}\"");
		}
		else if(tokenType == WORD && token == "use") {
			/*std::cout << "Parser: Using module \"" << expectNextToken(WORD) << "\"\n";*/
			std::string moduleName = expectNextToken(WORD);
			
			vault->set("::modules", moduleName);
			
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
						Util::StreamAsString() << currentModule << ":" << token, next);
					
					else vault->set(token, next);
					
					/*std::cout << "Set \"" << currentModule << "::" << token << "\" to \""
						<< next << "\" with operator " << op << std::endl;*/
				}
				else throw ParsingException("Invalid RHS");
				
				std::string sym = expectNextToken(SYMBOL);
				if(sym == ";") break;
				else if(sym == ",") {}
				else {
					throw ParsingException(Util::StreamAsString()
						<< "Expected \",\" or \";\", got \"" << sym << "\"");
				}
			} while(true);
		}
		else {
			throw ParsingException(Util::StreamAsString()
				<< "Syntax error at token \"" << token << "\"");
		}
		//std::cout << "\"" << token << "\"\n";
	}
	
	closeFile();
}

void Parser::parseDirectory(Vault *vault, const std::string &directory) {
	DIR *dir = opendir(directory.c_str());
	if(dir == NULL) return;
	
	vault->clear("::directory");
	vault->set("::directory", directory);
	
	struct dirent *dent;
	
	while((dent = readdir(dir)) != NULL) {
		if(!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) continue;
		std::string possible = directory + "/";
		possible += dent->d_name;
		
		struct stat s;
		if(stat(possible.c_str(), &s) != 0) {
			/* Cannot stat file, probably don't have access permissions. */
			continue;
		}
		
		possible += "/";
		
		if(S_ISDIR(s.st_mode)) {
			parse(vault, possible + AesalonModuleConfigFileName);
			std::string path = dent->d_name;
			path += ":root";
			vault->clear(path);
			vault->set(path, possible);
		}
	}
	
	closedir(dir);
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
		
		throw ParsingException(Util::StreamAsString()
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
		throw ParsingException(Util::StreamAsString()
			<< "Expected " << nameOf(expected) << " token, found " << nameOf(actual)
			<< " token: \"" << token << "\"");
	}
	
	return token;
}

void Parser::expectNextSymbol(const std::string &symbol) {
	std::string s = expectNextToken(SYMBOL);
	if(symbol != s) {
		throw ParsingException(Util::StreamAsString()
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
