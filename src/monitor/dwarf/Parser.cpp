#include "Message.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Parser::Parser(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    Block debug_info = elf_parser->get_section(".debug_info")->get_content();
    Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "debug_info.get_data(): " << debug_info.get_data());
    Word initial_length = 0;
    if((*debug_info[0] == 0xff) &&
        (*debug_info[1] == 0xff) &&
        (*debug_info[2] == 0xff) &&
        (*debug_info[3] == 0xff)) {
        dwarf_format = DWARF_64;
        debug_info.remove(0, 4);
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

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
