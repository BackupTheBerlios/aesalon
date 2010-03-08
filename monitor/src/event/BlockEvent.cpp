#include <iostream>
#include "BlockEvent.h"
#include "exception/EventException.h"
#include "misc/StreamAsString.h"
#include "elf/Parser.h"
#include "Initializer.h"
#include "misc/Message.h"

namespace Event {

/* serialization format:
    first bit is taken by BasicEvent
    second and third bits are block_type
    rest of the first byte is reserved
    next eight bytes are timestamp
    after that, depends on type . . .
*/
Block *BlockEvent::serialize() {
    Block *serialized = BasicEvent::serialize();
    
    serialized->get_data()[0] |= (block_type << 1) & 0x06;
    
    ELF::Parser *elf_parser = Initializer::get_instance()->get_program_manager()->get_elf_parser();
    ELF::Symbol *scope_symbol = elf_parser->get_symbol(get_scope_address());
    std::string scope_name = "";
    if(scope_symbol) {
        scope_name = scope_symbol->get_symbol_name();
        /*std::cout << "Event scope is \"" << scope_name << "\"\n";*/
        if(scope_name.size() >= 256) {
            Misc::Message(Misc::Message::WARNING_MESSAGE, Misc::StreamAsString() << "Truncating scope name \"" << scope_name << "\" to 255 characters");
            scope_name.erase(255);
        }
    }
    
    std::size_t start_position = serialized->get_size();
    serialized->resize(serialized->get_size() + 1 + scope_name.size());
    serialized->get_data()[start_position] = scope_name.size() & 0xff;
    start_position ++;
    for(std::size_t x = 0; x < scope_name.size(); x ++) {
        serialized->get_data()[start_position + x] = scope_name[x];
    }
    
    serialized->push_word(get_address());

    switch(get_block_type()) {
        case ALLOC_EVENT:
            serialized->push_word(get_size());
            break;
        case REALLOC_EVENT:
            /* NOTE: the new address first makes it easier to deserialize later */
            serialized->push_word(get_new_address());
            serialized->push_word(get_size());
            break;
        case FREE_EVENT: break;
        default:
            throw Exception::EventException("Asked to serialize invalid Block event");
    }
    
    return serialized;
}

} // namespace Event
