#include "AbbrevParser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

AbbrevParser::AbbrevParser(Misc::SmartPointer<ELF::Parser> elf_parser) {
    Misc::SmartPointer<Block> abbrev_data = elf_parser->get_section(".debug_abbrev")->get_content();
    
    
}

AbbrevParser::~AbbrevParser() {
    
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
