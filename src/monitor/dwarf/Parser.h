#ifndef AESALON_MONITOR_DWARF_PARSER_H
#define AESALON_MONITOR_DWARF_PARSER_H

#include "elf/Parser.h"

#include "EntryManager.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class Parser {
public:
    enum dwarf_format_e {
        DWARF_32,
        DWARF_64
    };
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
    Misc::SmartPointer<EntryManager> entry_manager;
    
    dwarf_format_e dwarf_format;
public:
    Parser(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~Parser() {}
    
    dwarf_format_e get_dwarf_format() const { return dwarf_format; }
    
    Word parse_leb128(Block &block, bool is_signed);
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
