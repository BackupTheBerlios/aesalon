/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/ArgumentParser.cpp

*/

#include <iostream>
#include <cstring>

#include "config/ArgumentParser.h"

namespace Monitor {
namespace Config {

int ArgumentParser::parse(Vault *vault, char **argv, int argc) {
	int arg;
	for(arg = 1; arg < argc; arg ++) {
		if(std::strcmp(argv[arg], "--") == 0) {
			arg ++;
			break;
		}
		else if(std::strcmp(argv[arg], "--use-module") == 0) {
			std::cout << "ArgumentParser: Using module " << argv[++arg] << std::endl;
		}
	}
	
	return arg;
}

} // namespace Config
} // namespace Monitor
