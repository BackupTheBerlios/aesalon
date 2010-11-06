/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/ArgumentParser.cpp

*/

#include <iostream>

#include "config/ArgumentParser.h"
#include "common/ParsingException.h"

namespace Monitor {
namespace Config {

ArgumentParser::ArgumentParser(Store *store, char **argv) : m_store(store), m_argv(argv) {
	
}

ArgumentParser::~ArgumentParser() {
	
}

int ArgumentParser::parse() {
	if(m_argv[0] == NULL) {
		throw Common::ParsingException("Malformed argv given.");
	}
	
	int i = 0;
	
	while(m_argv[++i] != NULL) {
		if(m_argv[i][0] != '-' || m_argv[i][1] != '-') {
			return i;
		}
		
		std::string argument = &m_argv[i][2];
		if(argument.length() == 0) {
			return i;
		}
		
		std::string::size_type divider = argument.find('=');
		Item *item = m_store->item(argument.substr(0, divider));
		if(divider == std::string::npos) item->setValue(true);
		else item->setValue(argument.substr(divider+1));
	}
	return i;
}

} // namespace Config
} // namespace Monitor
