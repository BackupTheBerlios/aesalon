/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/FileParser.cpp

*/

#include <fstream>

#include "config/FileParser.h"
#include "common/ParsingException.h"
#include "common/StreamAsString.h"

namespace Monitor {
namespace Config {

FileParser::FileParser(Store *store) : m_store(store) {

}

FileParser::~FileParser() {

}

void FileParser::parse(const std::string &filename) {
	std::ifstream file(filename.c_str(), std::ios_base::in);
	
	if(!file.is_open()) return;
	
	std::string group = "";
	
	while(!file.eof()) {
		std::string line;
		std::getline(file, line);
		
		/* If a line begins with a '#', then it is a comment. */
		if(line[0] == '#') continue;
		else if(line.length() == 0) continue;
		
		std::string::size_type divider = line.find(' ');
		
		std::string command = line.substr(0, divider);
		
		std::string content;
		if(divider != std::string::npos) content = line.substr(divider+1);
		
		if(command == "group") group = content;
		else if(command == "set") {
			divider = content.find("=");
			if(divider == std::string::npos)
				throw Common::ParsingException("Configuration file invalid: set command has no argument.");
			
			m_store->item(group, content.substr(0, divider))->setValue(content.substr(divider+1));
		}
		else {
			throw Common::ParsingException(Common::StreamAsString() << 
				"Configuration file invalid: unknown command \"" << command << "\".");
		}
	}
	
	file.close();
}

} // namespace Config
} // namespace Monitor
