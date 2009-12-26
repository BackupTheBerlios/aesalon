#include <iostream>
#include "CompilationUnit.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

CompilationUnit::CompilationUnit(Misc::SmartPointer<ELF::Parser> elf_parser) {
    Misc::SmartPointer<Block> debug_info = elf_parser->get_section(".debug_info")->get_content();
    unit_length = Parser::parse_u32(debug_info);
    if(unit_length == 0xffffffff) {
        unit_length = Parser::parse_u64(debug_info);
        dwarf_format = DWARF_64;
    }
    else dwarf_format = DWARF_32;
    
    dwarf_version = Parser::parse_u16(debug_info);
    
    if(get_dwarf_format() == DWARF_32) abbrev_offset = Parser::parse_u32(debug_info);
    else if(get_dwarf_format() == DWARF_64) abbrev_offset = Parser::parse_u64(debug_info);
    
    std::cout << ".dwarf_abbrev offset for compilation unit is: " << abbrev_offset << std::endl;
    
    address_size = Parser::parse_u8(debug_info);
    
    entry_manager = new EntryManager();
    
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
