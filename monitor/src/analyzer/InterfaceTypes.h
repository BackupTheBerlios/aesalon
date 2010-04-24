#ifndef AESALON_ANALYZER_INTERFACE_TYPES_H
#define AESALON_ANALYZER_INTERFACE_TYPES_H

#include <string>

#include "Types.h"

namespace Analyzer {
class Object {
private:
    std::string name;
    Word address;
    Word size;
public:
    Object(const std::string &name, Word address, Word size);
    ~Object();
    
    const std::string &get_name() const { return name; }
    Word get_address() const { return address; }
    Word get_size() const { return size; }
    
    bool operator<(const Object &other) const;
    bool operator<(Word address) const;
    bool operator>(Word address) const;
};

} // namespace Analyzer

#endif
