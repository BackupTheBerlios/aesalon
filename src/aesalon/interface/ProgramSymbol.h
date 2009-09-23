#ifndef AESALON_INTERFACE_PROGRAM_SYMBOL_H
#define AESALON_INTERFACE_PROGRAM_SYMBOL_H

#include <string>

namespace Aesalon {
namespace Interface {

class ProgramSymbol {
private:
    std::string symbol_name;
    std::size_t symbol_address;
public:
    ProgramSymbol(std::string name, std::size_t address) :
        symbol_name(name), symbol_address(address) {}
    
    void set_symbol_name(std::string new_name) { symbol_name = new_name; }
    std::string get_symbol_name() const { return symbol_name; }
    void set_symbol_address(std::size_t new_address) { symbol_address = new_address; }
    std::size_t get_symbol_address() const { return symbol_address; }
    
    bool operator==(ProgramSymbol &other) const {
        return get_symbol_address() == other.get_symbol_address();
    }
    
    bool operator<(ProgramSymbol &other) const {
        return get_symbol_address() < other.get_symbol_address();
    }
    
    
};

} // namespace Interface
} // namespace Aesalon

#endif
