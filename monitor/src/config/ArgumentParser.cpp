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
#include "config/ConcreteVault.h"

namespace Monitor {
namespace Config {

int ArgumentParser::parse(ConcreteVault *vault, char **argv) {
	int arg;
	for(arg = 1; argv[arg] != NULL; arg ++) {
		if(std::strcmp(argv[arg], "--") == 0) {
			arg ++;
			break;
		}
		else if(std::strcmp(argv[arg], "--set") == 0) {
			/*std::cout << argv[arg+1] << std::endl;*/
			std::string line = argv[arg+1];
			std::string::size_type divider = line.find('=');
			std::string key = line.substr(0, divider);
			std::string data = line.substr(divider+1);
			vault->clear(key);
			
			vault->set(key, data);
			arg ++;
		}
		else if(std::strcmp(argv[arg], "--append") == 0) {
			/*std::cout << argv[arg+1] << std::endl;*/
			std::string line = argv[arg+1];
			std::string::size_type divider = line.find('=');
			if(line[divider-1] != '+') {} // TODO: implement error
			std::string key = line.substr(0, divider-1);
			std::string data = line.substr(divider+1);
			vault->set(key, data);
			arg ++;
		}
		else if(std::strcmp(argv[arg], "--use-module") == 0) {
			std::cout << "ArgumentParser: Using module " << argv[++arg] << std::endl;
		}
		else if(std::strcmp(argv[arg], "--help") == 0) {
			vault->set("help", "true");
		}
		else break;
	}
	
	return arg;
}

} // namespace Config
} // namespace Monitor
