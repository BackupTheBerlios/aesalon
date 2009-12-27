#include "Disassembler.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

template<> Disassembler *Misc::Singleton<Disassembler>::instance = 0;

Disassembler::Disassembler(Misc::SmartPointer<ELF::Parser> elf_parser)
    : Misc::Singleton<Disassembler>(), elf_parser(elf_parser) {
    
}

Disassembler::~Disassembler() {
    
}

Misc::SmartPointer<Instruction> Disassembler::get_instruction_at(Platform::MemoryAddress address) const {
    for(instruction_vector_t::const_iterator i = instruction_vector.begin(); i != instruction_vector.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
