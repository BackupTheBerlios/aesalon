#ifndef AESALON_INTERFACE_DWARF_PARSER_H
#define AESALON_INTERFACE_DWARF_PARSER_H

#include "elf/Parser.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Parser {
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
public:
    Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {}
    virtual ~Parser() {}
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
