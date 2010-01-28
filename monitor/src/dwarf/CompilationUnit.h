#ifndef AESALON_MONITOR_DWARF_COMPILATION_UNIT_H
#define AESALON_MONITOR_DWARF_COMPILATION_UNIT_H

#include "Types.h"
#include "elf/Parser.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class CompilationUnit {
private:
    Word initial_offset;
public:
    CompilationUnit(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~CompilationUnit();
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
