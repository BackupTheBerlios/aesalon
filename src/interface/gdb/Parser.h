#ifndef AESALON_INTERFACE_GDB_PARSER_H
#define AESALON_INTERFACE_GDB_PARSER_H

#include <string>
#include "ParseData.h"
#include "String.h"

#include "misc/SmartPointer.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ParserException : public Misc::Exception {
public:
    ParserException(std::string message) : Misc::Exception(message) {}
};

class Parser {
private:
    std::string string;
    std::string::size_type begin;
    
    Misc::SmartPointer<ParseSequence> parse_sequence();
    Misc::SmartPointer<ParseResult> parse_result();
    Misc::SmartPointer<ParseData> parse_value();
    Misc::SmartPointer<ParseData> parse_string();
    Misc::SmartPointer<ParseData> parse_tuple();
    Misc::SmartPointer<ParseData> parse_list();
    
    std::string next_word();
    bool is_word_char(char to_test);
public:
    Parser() {}
    virtual ~Parser() {}
    
    Misc::SmartPointer<String> parse_gdb_string(std::string string);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
