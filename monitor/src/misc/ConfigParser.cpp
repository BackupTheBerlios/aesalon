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
        config_map[config_name] = buf_string;
    }
    
    delete[] buffer;
    
    stream.close();
}

} // namespace Misc
