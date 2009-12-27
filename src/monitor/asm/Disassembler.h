#ifndef AESALON_MONITOR_ASM_DISASSEMBLER_H
#define AESALON_MONITOR_ASM_DISASSEMBLER_H

#include <string>
#include <vector>

#include "elf/Parser.h"

#include "Instruction.h"

#include "misc/Singleton.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Disassembler : public Misc::Singleton<Disassembler> {
protected:
    typedef std::vector<Misc::SmartPointer<Instruction> > instruction_vector_t;
private:
    Misc::SmartPointer<ELF::Parser> elf_parser;
    
    instruction_vector_t instruction_vector;
public:
    Disassembler(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~Disassembler();
    
    Misc::SmartPointer<Instruction> get_instruction_at(Platform::MemoryAddress address) const;
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
