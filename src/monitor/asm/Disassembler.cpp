#include <iostream>
#include "Disassembler.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Disassembler::Disassembler(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    
}

Misc::SmartPointer<InstructionList> Disassembler::get_symbol_il(std::string symbol_name) {
    if(symbol_to_il[symbol_name] != NULL) return symbol_to_il[symbol_name];
    
    std::cout << "Disassembling symbol \"" << symbol_name << "\"\n";
    
    std::cout << std::hex;
    
    Misc::SmartPointer<ELF::Symbol> symbol = elf_parser->get_symbol(symbol_name);
    Word section_offset = elf_parser->get_section(".text")->get_virtual_address();
    
    Word local_symbol_address = symbol->get_address() - section_offset;
    
    Misc::SmartPointer<Block> symbol_block = elf_parser->get_section(".text")->get_content()
        ->subset(local_symbol_address, local_symbol_address + symbol->get_size());
    
    Misc::SmartPointer<InstructionList> il = new InstructionList(symbol->get_address());
    
    while(symbol_block->get_size() > 0) {
        il->add_instruction(new Instruction(symbol_block));
    }
    
    std::cout << std::dec;
    
    return NULL;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
