#ifndef AESALON_INTERFACE_DWARF_PARSER_H
#define AESALON_INTERFACE_DWARF_PARSER_H

#include "elf/Parser.h"

#include "EntryManager.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Parser {
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
    
    Misc::SmartPointer<EntryManager> entry_manager;
public:
    Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {}
    virtual ~Parser() {}
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
