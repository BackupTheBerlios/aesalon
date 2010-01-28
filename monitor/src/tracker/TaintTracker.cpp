#include "TaintTracker.h"
#include "ptrace/BreakpointReference.h"
#include "Initializer.h"




void TaintTracker::handle_breakpoint(const PTrace::BreakpointReference &breakpoint) {
    
}

void TaintTracker::place_breakpoints(std::string symbol) {
    ASM::InstructionList *il = disassembler->get_symbol_il(symbol);
    
    std::size_t instruction_count = il->get_instruction_count();
    for(std::size_t x = 0; x < instruction_count; x ++) {
        ASM::Instruction *instruction = il->get_instruction_by_index(x);
        if(instruction->changes_memory()) {
            Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->place_breakpoint(instruction->get_address(), this);
        }
    }
}

TaintedData *TaintTracker::get_tainted_data(Word address) const {
    tainted_data_t::const_reverse_iterator i = tainted_data.rbegin();
    
    for(; i != tainted_data.rend(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    
    return NULL;
}



