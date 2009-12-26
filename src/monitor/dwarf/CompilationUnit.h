#ifndef AESALON_MONITOR_DWARF_COMPILATION_UNIT_H
#define AESALON_MONITOR_DWARF_COMPILATION_UNIT_H

#include "Types.h"
#include "elf/Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class CompilationUnit {
public:
    enum dwarf_format_e {
        DWARF_32,
        DWARF_64
    };
private:
    dwarf_format_e dwarf_format;
    Word unit_length;
    Word dwarf_version;
    Word abbrev_offset;
    Word address_size;
public:
    CompilationUnit(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~CompilationUnit() {}
    
    dwarf_format_e get_dwarf_format() const { return dwarf_format; }
    
    Word get_unit_length() const { return unit_length; }
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
