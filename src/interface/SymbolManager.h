#ifndef AESALON_INTERFACE_SYMBOL_MANAGER_H
#define AESALON_INTERFACE_SYMBOL_MANAGER_H

#include <vector>

#include "Symbol.h"

#include "misc/SmartPointer.h"
#include "platform/BidirectionalPipe.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {

class SymbolManager {
public:
    typedef std::vector<Misc::SmartPointer<Symbol> > symbol_vector_t;
private:
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    symbol_vector_t symbol_list;
    symbol_vector_t global_list;
    
    std::string executable_name;
    
    const std::string nm_path;
    std::string demangle_style;
    
    Platform::MemoryAddress symbol_address;
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
};

} // namespace Interface
} // namespace Aesalon

#endif
