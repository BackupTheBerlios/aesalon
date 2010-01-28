#ifndef AESALON_MONITOR_ELF_SYMBOL_H
#define AESALON_MONITOR_ELF_SYMBOL_H

#include <string>

#include "Types.h"





class Symbol {
private:
    std::string symbol_name;
    Word address;
    Word size;
public:
    Symbol(std::string symbol_name, Word address, Word size) : symbol_name(symbol_name), address(address), size(size) {}
    virtual ~Symbol() {}
    
    Word get_address() const { return address; }
    std::string get_symbol_name() const { return symbol_name; }
    Word get_size() const { return size; }
};





#endif
