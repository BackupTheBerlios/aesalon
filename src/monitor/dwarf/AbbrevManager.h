#ifndef AESALON_MONITOR_DWARF_ABBREV_MANAGER_H
#define AESALON_MONITOR_DWARF_ABBREV_MANAGER_H

#include "elf/Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class AbbrevManager {
public:
    AbbrevManager(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~AbbrevManager();
    
    
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
