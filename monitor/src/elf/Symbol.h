#ifndef AESALON_MONITOR_ELF_SYMBOL_H
#define AESALON_MONITOR_ELF_SYMBOL_H

#include <string>

#include "Types.h"
#include "StorageOffset.h"

namespace ELF {

class Symbol {
private:
    StorageOffset storage_offset;
    std::string symbol_name;
    Word address;
    Word size;
public:
    Symbol(StorageOffset storage_offset, std::string symbol_name, Word address, Word size) : storage_offset(storage_offset), symbol_name(symbol_name), address(address), size(size) {}
    virtual ~Symbol() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    Word get_address() const { return address; }
    std::string get_symbol_name() const { return symbol_name; }
    Word get_size() const { return size; }
};

} // namespace ELF

#endif
