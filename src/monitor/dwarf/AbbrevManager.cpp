#include "AbbrevManager.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

AbbrevManager::AbbrevManager(Misc::SmartPointer<ELF::Parser> elf_parser) {
    Misc::SmartPointer<ELF::Section> debug_abbrev = elf_parser->get_section(".debug_abbrev");
}

AbbrevManager::~AbbrevManager() {
    
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
