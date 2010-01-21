#include "TaintTracker.h"
#include "ptrace/BreakpointReference.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {

void TaintTracker::handle_breakpoint(const PTrace::BreakpointReference &breakpoint) {
    
}

void TaintTracker::place_breakpoints(std::string symbol) {
    Misc::SmartPointer<ASM::InstructionList> il = disassembler->get_symbol_il(symbol);
    
    std::size_t instruction_count = il->get_instruction_count();
    for(std::size_t x = 0; x < instruction_count; x ++) {
        Misc::SmartPointer<ASM::Instruction> instruction = il->get_instruction_by_index(x);
        if(instruction->changes_memory()) {
            Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->place_breakpoint(instruction->get_address(), this);
        }
    }
}

} // namespace Monitor
} // namespace Aesalon
