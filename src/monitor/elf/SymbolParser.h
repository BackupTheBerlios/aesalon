#ifndef AESALON_MONITOR_ELF_SYMBOL_PARSER_H
#define AESALON_MONITOR_ELF_SYMBOL_PARSER_H

#include "Symbol.h"

namespace Aesalon {
namespace Monitor {
namespace ELF {

class Parser;

class SymbolParser {
protected:
    typedef std::vector<Misc::SmartPointer<Symbol> > symbol_vector_t;
private:
    Misc::SmartPointer<Parser> elf_parser;
    symbol_vector_t symbol_vector;
public:
    SymbolParser(Misc::SmartPointer<Parser> elf_parser);
    virtual ~SymbolParser() {}
    
    Misc::SmartPointer<Symbol> get_symbol(std::string name) const;
    
    void dump_symbols() const;
};

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon

#endif
