#ifndef AESALON_MONITOR_ASM_DISASSEMBLER_H
#define AESALON_MONITOR_ASM_DISASSEMBLER_H

#include <map>

#include "elf/Parser.h"
#include "elf/Symbol.h"
#include "InstructionList.h"
#include "BidirectionalPipe.h"

namespace ASM {

class Disassembler {
public:
    typedef std::map<std::string, InstructionList *> symbol_to_il_t;
private:
    ELF::Parser *elf_parser;
    
    symbol_to_il_t symbol_to_il;
    
    BidirectionalPipe *bi_pipe;
    
    void parse_objdump_output();
public:
    Disassembler(ELF::Parser *elf_parser);
    virtual ~Disassembler() {}
    
    InstructionList *get_symbol_il(std::string symbol_name)
        { return symbol_to_il[symbol_name]; }
};

} // namespace ASM

#endif
