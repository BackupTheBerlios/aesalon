#include "AbbrevParser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

AbbrevParser::AbbrevParser(Misc::SmartPointer<ELF::Parser> elf_parser) {
    Misc::SmartPointer<Block> abbrev_data = elf_parser->get_section(".debug_abbrev")->get_content();
    
    tag_list.push_back(new Tag("NULL spaceholder tag"));
    while(abbrev_data->get_size()) {
        tag_list.push_back(Tag::parse_from(abbrev_data));
    }
}

AbbrevParser::~AbbrevParser() {
    
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
