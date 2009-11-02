#include <sstream>

#include "Parser.h"
#include "String.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Misc::SmartPointer<String> Parser::parse_gdb_string(std::string string) {
    this->string = string;
    if(this->string.find("\n") != std::string::npos) {
        this->string.erase(string.length() - 1);
    }
    
    String *instance;
    
    int digits = 0;
    while(std::isdigit(this->string[digits])) digits ++;
    
    char type = this->string[digits];
    begin = digits + 1;
    
    if(std::string("~@&").find(type) != std::string::npos) {
        instance = new StreamOutput("");
    }
    else if(std::string("*+=").find(type) != std::string::npos) {
        Misc::SmartPointer<ParseSequence> seq = parse_data_sequence();
        
        instance = new AsyncOutput(seq);
    }
    else if(type == '^') {
        Misc::SmartPointer<ParseSequence> seq = parse_data_sequence();
        
        instance = new ResultRecord(seq);
    }
    
    if(digits) {
        std::stringstream ss;
        ss << this->string.substr(0, digits);
        int digits_value;
        ss >> digits_value;
        instance->set_number(new NumberedString(digits_value));
    }
    
    return instance;
}

Misc::SmartPointer<ParseSequence> Parser::parse_data_sequence() {
    return NULL;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
