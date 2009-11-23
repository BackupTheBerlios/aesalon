#ifndef AESALON_PLATFORM_SYMBOL_H
#define AESALON_PLATFORM_SYMBOL_H

#include <string>
#include <iostream>
#include "MemoryAddress.h"

namespace Aesalon {
namespace Platform {

class Symbol {
public:
    enum symbol_type_e {
        FUNCTION,
        VARIABLE
    };
private:
    MemoryAddress address;
    MemoryAddress size;
    symbol_type_e type;
    std::string name;
    bool parsed;
public:
    Symbol(Platform::MemoryAddress address, Platform::MemoryAddress size, symbol_type_e type, std::string name) :
        address(address), size(size), type(type), name(name), parsed(false) {}
    virtual ~Symbol() {}
    
    
    std::string get_name() const { return name; }
    symbol_type_e get_type() const { return type; }
    MemoryAddress get_address() const { return address; }
    MemoryAddress get_size() const { return size; }
    void set_size(MemoryAddress new_size) { size = new_size; }
    
    bool is_parsed() const { return parsed; }
    void set_parsed(bool value) { parsed = value; }
};

} // namespace Platform
} // namespace Aesalon

#endif
