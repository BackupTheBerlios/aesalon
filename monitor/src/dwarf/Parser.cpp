#include <iostream>
#include "Message.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Parser::Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    Misc::SmartPointer<ELF::Section> debug_info_section = elf_parser->get_section(".debug_info");
    Misc::SmartPointer<Block> debug_info = elf_parser->get_section(".debug_info")->get_content();
}

Word Parser::parse_uleb128(Misc::SmartPointer<Block> block) {
    Word result = 0;
    int offset = 0;
    
    while(true) {
        Byte next = *block->get_data(offset);
        result |= (next & 0x7f) << (offset * 7);
        if(next & 0x80) break;
        offset ++;
    }
    
    block->remove(0, offset);
    
    return result;
}

SWord Parser::parse_sleb128(Misc::SmartPointer<Block> block) {
    SWord result = 0;
    /*int offset = 0;*/
    
    return result;
}

Word Parser::parse_u8(Misc::SmartPointer<Block> block) {
    Word value = 0;
    value = (*block->get_data(0));
    block->remove(0, 1);
    return value;
}


Word Parser::parse_u16(Misc::SmartPointer<Block> block) {
    Word value = 0;
    value = (*block->get_data(0));
    value += (*block->get_data(1)) << 8;
    block->remove(0, 2);
    return value;
}

Word Parser::parse_u32(Misc::SmartPointer<Block> block) {
    Word value = 0;
    value = (*block->get_data(0));
    value += (*block->get_data(1)) << 8;
    value += (*block->get_data(2)) << 16;
    value += (*block->get_data(3)) << 24;
    block->remove(0, 4);
    return value;
}

Word Parser::parse_u64(Misc::SmartPointer<Block> block) {
    Word value = 0;
    value = (*block->get_data(0));
    value += Word(*block->get_data(1)) << 8;
    value += Word(*block->get_data(2)) << 16;
    value += Word(*block->get_data(3)) << 24;
    value += Word(*block->get_data(4)) << 32;
    value += Word(*block->get_data(5)) << 40;
    value += Word(*block->get_data(6)) << 48;
    value += Word(*block->get_data(7)) << 56;
    
    block->remove(0, 8);
    return value;
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
