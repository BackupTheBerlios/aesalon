#ifndef AESALON_MONITOR_DWARF_PARSER_H
#define AESALON_MONITOR_DWARF_PARSER_H

#include <vector>

#include "elf/Parser.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class Parser {
public:
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
public:
    Parser(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~Parser() {}
    
    static Word parse_uleb128(Misc::SmartPointer<Block> block);
    static SWord parse_sleb128(Misc::SmartPointer<Block> block);
    
    static Word parse_u8(Misc::SmartPointer<Block> block);
    static Word parse_u16(Misc::SmartPointer<Block> block);
    static Word parse_s16(Misc::SmartPointer<Block> block);
    static Word parse_u32(Misc::SmartPointer<Block> block);
    static Word parse_s32(Misc::SmartPointer<Block> block);
    static Word parse_u64(Misc::SmartPointer<Block> block);
    static Word parse_s64(Misc::SmartPointer<Block> block);
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
