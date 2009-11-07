#ifndef AESALON_INTERFACE_SYMBOL_H
#define AESALON_INTERFACE_SYMBOL_H

#include <string>
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {

class Symbol {
public:
    enum symbol_type_e {
        FUNCTION,
        VARIABLE
    };
private:
    Platform::MemoryAddress address;
    symbol_type_e type;
    std::string name;
public:
    Symbol(Platform::MemoryAddress address, symbol_type_e type, std::string name) : address(address), type(type), name(name) {}
    virtual ~Symbol() {}
    
    
    std::string get_name() const { return name; }
    symbol_type_e get_type() const { return type; }
    Platform::MemoryAddress get_address() const { return address; }
};

} // namespace Interface
} // namespace Aesalon

#endif
