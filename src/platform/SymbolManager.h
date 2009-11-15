#ifndef AESALON_PLATFORM_SYMBOL_MANAGER_H
#define AESALON_PLATFORM_SYMBOL_MANAGER_H

#include <vector>

#include "Symbol.h"

#include "misc/SmartPointer.h"
#include "BidirectionalPipe.h"
#include "MemoryAddress.h"

namespace Aesalon {
namespace Platform {

class SymbolManager {
public:
    typedef std::vector<Misc::SmartPointer<Symbol> > symbol_vector_t;
private:
    Misc::SmartPointer<BidirectionalPipe> bi_pipe;
    symbol_vector_t symbol_list;
    symbol_vector_t global_list;
    
    std::string executable_name;
    
    const std::string nm_path;
    std::string demangle_style;
    
    MemoryAddress symbol_address;
    char symbol_type;
    std::string symbol_name;
    
    void setup_pipe();
    
    void parse_nm_output();
    
    void handle_symbol();
    void handle_variable();
public:
    SymbolManager();
    virtual ~SymbolManager();
    
    void parse_from_executable(std::string executable_name);
    
    Misc::SmartPointer<Symbol> get_symbol(symbol_vector_t::size_type which) const {
        return symbol_list.at(which);
    }
    symbol_vector_t::size_type get_symbols() const { return symbol_list.size(); }
    Misc::SmartPointer<Symbol> get_global(symbol_vector_t::size_type which) const {
        return global_list.at(which);
    }
    symbol_vector_t::size_type get_globals() const { return global_list.size(); }
};

} // namespace Platform
} // namespace Aesalon

#endif
