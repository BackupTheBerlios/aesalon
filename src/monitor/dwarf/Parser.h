#ifndef AESALON_MONITOR_DWARF_PARSER_H
#define AESALON_MONITOR_DWARF_PARSER_H

#include "elf/Parser.h"

#include "EntryManager.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class Parser {
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
    
    Misc::SmartPointer<EntryManager> entry_manager;
public:
    Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {}
    virtual ~Parser() {}
    
    Word parse_leb128(Block block, bool is_signed);
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
