#include <sstream>
#include <iostream>

#include "Parser.h"
#include "String.h"

#include "misc/SmartPointer.h"
#include "misc/String.h"

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
        std::string stream = Misc::String::remove_escapes(this->string.substr(begin));
        if(stream.find("\n") != std::string::npos) stream[stream.find("\n")] = 0;
        instance = new StreamOutput(stream);
    }
    else if(std::string("*+=").find(type) != std::string::npos) {
        Misc::SmartPointer<ParseSequence> seq = parse_sequence();
        
        instance = new AsyncOutput(seq);
    }
    else if(type == '^') {
        Misc::SmartPointer<ParseSequence> seq = parse_sequence();
        
        instance = new ResultRecord(seq);
    }
    
    if(digits) {
        std::stringstream ss;
        ss << this->string.substr(0, digits);
        int digits_value;
        ss >> digits_value;
        instance->set_number(new NumberedString(digits_value));
        std::cout << "instance->set_number: " << digits_value << std::endl;
    }
    else instance->set_number(NULL);
    
    return instance;
}

Misc::SmartPointer<ParseSequence> Parser::parse_sequence() {
    ParseSequence *seq = new ParseSequence(next_word());
    
    while(begin < string.length()) {
        if(string[begin] == ',') begin ++;
        else throw ParserException("Parsing sequence; expected ',' or newline");
        
        seq->add_element(parse_result());
    }
    
    return seq;
}

Misc::SmartPointer<ParseResult> Parser::parse_result() {
    /* Results are of the form:
        RESULT: VARIABLE "=" VALUE
    */

    std::string name = next_word();
    
    if(string[begin] != '=') throw ParserException("Parsing result, expected '='");
    begin ++;
    
    return new ParseResult(name, parse_value());
}

Misc::SmartPointer<ParseData> Parser::parse_value() {
    /* '"' denotes a string */
    if(string[begin] == '"') {
        begin ++;
        return parse_string();
    }
    /* '{' denotes a tuple */
    else if(string[begin] == '{') {
        begin ++;
        return parse_tuple();
    }
    /* '[' denotes a list */
    else if(string[begin] == '[') {
        begin ++;
        return parse_list();
    }
    else throw ParserException(Misc::StreamAsString()
        << "Parsing value, unexpected character '" << string[begin] << "'");
}

Misc::SmartPointer<ParseData> Parser::parse_string() {
    std::string::size_type position = begin;
    while(position < string.length()) {
        if(string[position] == '"' && !Misc::String::is_escaped(string, position)) break;
        position ++;
    }
    
    std::string data = Misc::String::remove_escapes(string.substr(begin, position - begin));
    
    begin = position + 1;
    
    return new ParseString(data);
}

Misc::SmartPointer<ParseData> Parser::parse_tuple() {
    ParseTuple *tuple = new ParseTuple();
    bool first_item = true;
    
    while(begin < string.length()) {
        if(string[begin] == '}') {
            begin ++;
            break;
        }
        else if(first_item) first_item = false;
        else if(string[begin] == ',') begin ++;
        else throw ParserException("Parsing tuple, expected '}' or ','");
        
        tuple->add_element(parse_result());
    }
    
    return tuple;
}

Misc::SmartPointer<ParseData> Parser::parse_list() {
    ParseList *list = new ParseList;
    bool first_item = true;
    
    while(begin < string.length()) {
        if(string[begin] == ']') {
            begin ++;
            break;
        }
        else if(first_item) first_item = false;
        else if(string[begin] == ',') begin ++;
        else throw ParserException("Parsing list, expected ']' or ','");
        
        /* If the first character is alpha, then it's of the form variable=value, or a result.
            Otherwise, it's going to be '{' or '[', denoting a tuple or list. */
        if(std::isalpha(string[begin])) list->add_element(parse_result().to<ParseData>());
        else list->add_element(parse_value());
    }
    
    return NULL;
}

std::string Parser::next_word() {
    std::string::size_type position = begin;
    while(position < string.length() && is_word_char(string[position])) position ++;
    
    if(position == begin) {
        /* Expected gdb word chars . . . */
        throw ParserException("Expected word characters, but found none.");
    }
    
    std::string word = string.substr(begin, position - begin);
    
    begin = position;
    
    return word;
}

bool Parser::is_word_char(char to_test) {
    if(std::isalpha(to_test) || to_test == '-' || to_test == '_') return true;
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
