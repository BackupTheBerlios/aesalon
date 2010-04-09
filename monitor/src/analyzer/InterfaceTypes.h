#ifndef AESALON_ANALYZER_INTERFACE_TYPES_H
#define AESALON_ANALYZER_INTERFACE_TYPES_H

#include <string>

#include "Types.h"

namespace Analyzer {

class Symbol {
private:
    std::string name;
    Word address;
    Word size;
public:
    Symbol(const std::string &name, Word address, Word size);
    ~Symbol();
    
    const std::string &get_name() const { return name; }
    Word get_address() const { return address; }
    Word get_size() const { return size; }
};

} // namespace Analyzer

#endif
