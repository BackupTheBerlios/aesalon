/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file misc/ConfigParser.cpp
*/

#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "ConfigParser.h"
#include "Message.h"
#include "StreamAsString.h"

namespace Misc {

ConfigParser::ConfigParser() {
    parse_config_file(SYS_CONFIG_PATH);
    parse_config_file(USER_CONFIG_PATH);
}

ConfigParser::~ConfigParser() {
    
}

void ConfigParser::parse_config_file(std::string filename) {
    if(filename.find("~") != std::string::npos) {
        filename.replace(filename.find("~"), 1, std::string(getenv("HOME")));
    }
    std::ifstream stream(filename.c_str());
    
    if(!stream.is_open()) {
        /*Message::Message(Message::WARNING_MESSAGE, Misc::StreamAsString() << "Couldn't open configuration file \"" << filename << "\"");*/
        return;
    }
    
    char *buffer = new char[1024];
    
    while(!stream.eof()) {
        stream.getline(buffer, 1024, '\n');
        std::string buf_string = buffer;
        if(buf_string.find("=") == std::string::npos) continue;
        std::string config_name = buf_string.substr(0, buf_string.find("="));
        buf_string = buf_string.substr(buf_string.find("=")+1);
        if(buf_string[0] == '~') buf_string.replace(0, 1, std::string(getenv("HOME")));
        config_map[config_name] = buf_string;
    }
    
    delete[] buffer;
    
    stream.close();
}

} // namespace Misc
