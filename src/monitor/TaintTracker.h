#ifndef AESALON_MONITOR_TAINT_TRACKER_H
#define AESALON_MONITOR_TAINT_TRACKER_H

#include "asm/Instruction.h"
#include "asm/InstructionList.h"
#include "asm/Disassembler.h"
#include "ptrace/BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {

class TaintTracker : public PTrace::BreakpointObserver {
private:
    Misc::SmartPointer<ASM::Disassembler> disassembler;
public:
    TaintTracker(Misc::SmartPointer<ASM::Disassembler> disassembler) : disassembler(disassembler) {}
    virtual ~TaintTracker() {}
    
    virtual void handle_breakpoint(const PTrace::BreakpointReference &breakpoint);
    
    void place_breakpoints(std::string symbol);
};

} // namespace Monitor
} // namespace Aesalon

#endif
