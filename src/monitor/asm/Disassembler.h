#ifndef AESALON_MONITOR_ASM_DISASSEMBLER_H
#define AESALON_MONITOR_ASM_DISASSEMBLER_H

#include <map>

#include "elf/Parser.h"
#include "elf/Symbol.h"
#include "InstructionList.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Disassembler {
public:
    typedef std::map<std::string, Misc::SmartPointer<InstructionList> > symbol_to_il_t;
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
    
    symbol_to_il_t symbol_to_il;
    
    Misc::SmartPointer<Instruction> parse_instruction(Misc::SmartPointer<Block>);
public:
    Disassembler(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~Disassembler() {}
    
    Misc::SmartPointer<InstructionList> get_symbol_il(std::string symbol_name);
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
