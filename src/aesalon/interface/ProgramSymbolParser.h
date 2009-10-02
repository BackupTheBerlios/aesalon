#ifndef AESALON_INTERFACE_PROGRAM_SYMBOL_PARSER_H
#define AESALON_INTERFACE_PROGRAM_SYMBOL_PARSER_H

#include <string>
#include <vector>
#include <iostream>

#include "misc/SmartPointer.h"
#include "ProgramSymbol.h"

namespace Aesalon {
namespace Interface {

class ProgramSymbolParser {
public:
    typedef std::vector<Misc::SmartPointer<ProgramSymbol> > symbol_vector_t;
private:
    std::string filename;
    symbol_vector_t symbol_vector;
    void parse_line(std::string line);
public:
    ProgramSymbolParser(std::string filename) : filename(filename) {}
    virtual ~ProgramSymbolParser();
    
    std::string get_filename() const { return filename; }
    
    void parse();
    
    std::size_t get_address_by_number(std::size_t number) {
        if(number < symbol_vector.size() && symbol_vector.at(number).is_valid())
            return symbol_vector.at(number)->get_symbol_address();
        else return 0;
    }
    std::string find_name_by_address(std::size_t address);
};

} // namespace Interface
} // namespace Aesalon


#endif
