/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/ArgumentParser.cpp
*/

#include <cstring>
#include <iostream>

#include "monitor/ArgumentParser.h"
#include "config/Parser.h"
#include "util/MessageSystem.h"

namespace Monitor {

int ArgumentParser::parse(Config::Vault *vault, char **argv) {
	int arg;
	for(arg = 1; argv[arg] != NULL; arg ++) {
		if(std::strcmp(argv[arg], "--") == 0) {
			arg ++;
			break;
		}
		else if(std::strcmp(argv[arg], "--set") == 0) {
			if(argv[++arg] == NULL) {
				Message(Warning, "Expected argument to --set.");
				continue;
			}
			/*std::cout << argv[arg+1] << std::endl;*/
			std::string line = argv[arg];
			std::string::size_type divider = line.find('=');
			std::string key = line.substr(0, divider);
			std::string data = line.substr(divider+1);
			if(key[key.length()-1] != '+') vault->clear(key);
			else key.erase(key.length()-1);
			
			vault->set(key, data);
		}
		else if(std::strcmp(argv[arg], "--use-module") == 0) {
			if(argv[++arg] == NULL) {
				Message(Warning, "Expected argument to --use-module.");
				continue;
			}
			vault->set("::modules", argv[arg]);
			/*std::cout << "ArgumentParser: Using module " << argv[++arg] << std::endl;*/
		}
		else if(std::strcmp(argv[arg], "--search") == 0) {
			Config::Parser().parseDirectory(vault, argv[++arg]);
		}
		else if(std::strcmp(argv[arg], "--help") == 0 ||
			std::strcmp(argv[arg], "--usage") == 0) {
			vault->set("::help", "true");
		}
		else if(std::strcmp(argv[arg], "--version") == 0) {
			vault->set("::version", "true");
		}
		else if(std::strcmp(argv[arg], "--list-attributes") == 0) {
			vault->set("::list-attributes", "true");
		}
		else if(std::strcmp(argv[arg], "--output") == 0) {
			if(argv[++arg] == NULL) {
				Message(Warning, "Expected argument to --output.");
				continue;
			}
			vault->set("monitor:output", argv[arg]);
		}
		else if(std::strncmp(argv[arg], "--", 2) == 0) {
			Message(Warning, "Unknown argument \"" << argv[arg] << "\".");
		}
		else break;
	}
	
	return arg;
}

} // namespace Monitor
