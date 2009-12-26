#include <iostream>
#include "Message.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Parser::Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    Misc::SmartPointer<ELF::Section> debug_info_section = elf_parser->get_section(".debug_info");
    Misc::SmartPointer<Block> debug_info = elf_parser->get_section(".debug_info")->get_content();
    /*Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "debug_info.get_data(): " << debug_info.get_data());*/
    Word initial_length = 0;
    if((debug_info->get_data()[0] == 0xff) &&
        (debug_info->get_data()[1] == 0xff) &&
        (debug_info->get_data()[2] == 0xff) &&
        (debug_info->get_data()[3] == 0xff)) {
        dwarf_format = DWARF_64;
        debug_info->remove(0, 4);
        
        Message(Message::DEBUG_MESSAGE, "DWARF data is in DWARF_64 format");
    }
    else {
        initial_length = parse_u32(debug_info);
        
        Message(Message::DEBUG_MESSAGE, "DWARF data is in DWARF_32 format");
        Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "DWARF initial length is " << initial_length);
    }
    
    
}

Word Parser::parse_leb128(Block &block, bool is_signed) {
    Word result = 0;
    int shift = 0;
    Word offset = 0;
    
    while(true) {
        if(is_signed) {
            break;
        }
        else {
            Byte next = *block[offset++];
            result |= (next & 0x7f) << shift;
            if(next & 0x80) break;
        }
        shift += 7;
    }
    
    block.remove(0, offset);
    
    return result;
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

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
