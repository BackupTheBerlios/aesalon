#include <iostream>
#include "Disassembler.h"
#include "platform/BidirectionalPipe.h"
#include "misc/ArgumentList.h"
#include "misc/String.h"
#include "Message.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Disassembler::Disassembler(Misc::SmartPointer<ELF::Parser> elf_parser) : elf_parser(elf_parser) {
    Misc::ArgumentList al;
    al.add_argument("/usr/bin/objdump"); /* NOTE: hardcoded path . . . */
    al.add_argument("-dMintel");
    al.add_argument("--section=.text");
    al.add_argument(elf_parser->get_filename());
    
    bi_pipe = new Platform::BidirectionalPipe(al, true);
    
    Message::Message(Aesalon::Monitor::Message::DEBUG_MESSAGE, "Beginning disassembly of target");
    parse_objdump_output();
    Message::Message(Aesalon::Monitor::Message::DEBUG_MESSAGE, "Disassembly of target completed");
    bi_pipe = NULL;
}

void Disassembler::parse_objdump_output() {
    std::string line;
    Misc::SmartPointer<ELF::Symbol> symbol = NULL;
    while(bi_pipe->is_open()) {
        line = bi_pipe->get_string();
        if(line == "") continue;
        
        /*std::cout << "parsing objdump line \"" << line << "\"\n";*/
        
        Word address = 0;
        Misc::String::to<Word>(line, address, true);
        if(address < elf_parser->get_section(".text")->get_virtual_address()) continue;
        
        line.erase(0, line.find(" "));
        while(line[0] == ' ') line.erase(0, 1);
        
        if(line[0] == '<') {
            line.erase(0, 1);
            line.erase(line.find(">"));
            /* NOTE: ignores all symbols beginning with __ . . . */
            if(line.substr(0, 2) == "__") symbol = NULL;
            else {
                symbol = elf_parser->get_symbol(line);
                /*std::cout << "\tSymbol name is \"" << line << "\"\n";*/
            }
            continue;
        }
        /* it's an instruction . . . */
        if(!symbol.is_valid()) continue; /* Continue if there's no resolved symbol ATM . . . */
        if(line.find("<") != std::string::npos) line.erase(line.find("<"));
        bool finished = false;
        while(!finished) {
            Byte value = 0;
            Misc::String::to<Byte>(line.substr(0, 2), value, true);
            if(value == 0) break;
            line.erase(0, 1);
        }
        while(line.length() && line[line.length()-1] == ' ') line.erase(line.length()-1);
        while(line.length() && (line[0] == ' ' || line[0] == '\t')) {
            line.erase(0, 1);
        }
        if(line == "") continue;
        /*std::cout << "Assembly instruction is \"" << line << "\"\n";*/
        /* Now parse the instruction and push it onto the InstructionList for the symbol . . . */
        if(!symbol_to_il[symbol->get_symbol_name()].is_valid())
            symbol_to_il[symbol->get_symbol_name()] = new InstructionList(symbol->get_address());
        
        symbol_to_il[symbol->get_symbol_name()]->add_instruction(new Instruction(line, address));
    }
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
