#include <iostream>
#include "Message.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Parser::Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    Misc::SmartPointer<ELF::Section> debug_info_section = elf_parser->get_section(".debug_info");
    Misc::SmartPointer<Block> debug_info = elf_parser->get_section(".debug_info")->get_content();
#if 0
    /*Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "debug_info.get_data(): " << debug_info.get_data());*/
    Word initial_length = parse_u32(debug_info);
    
    /* According to the DWARF 3 specification (section 7.4), the format is 64-bit if the first four bits are 0xff. */
    if(initial_length == 0xffffffff) {
        dwarf_format = DWARF_64;
        Message(Message::DEBUG_MESSAGE, "DWARF .debug_info data is in DWARF_64 format");
        initial_length = parse_u64(debug_info);
    }
    else {
        /* Else it's a 32-bit DWARF file. */
        dwarf_format = DWARF_32;
        
        Message(Message::DEBUG_MESSAGE, "DWARF .debug_info data is in DWARF_32 format");
    }
    Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "DWARF .debug_info initial length is " << initial_length);
    
#endif
}

Word Parser::parse_uleb128(Block &block) {
    Word result = 0;
    int shift = 0;
    Word offset = 0;
    
    while(true) {
        Byte next = *block[offset++];
        result |= (next & 0x7f) << shift;
        if(next & 0x80) break;
        shift += 7;
    }
    
    block.remove(0, offset);
    
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
