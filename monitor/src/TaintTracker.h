#ifndef AESALON_MONITOR_TAINT_TRACKER_H
#define AESALON_MONITOR_TAINT_TRACKER_H

#include <vector>

#include "asm/Instruction.h"
#include "asm/InstructionList.h"
#include "asm/Disassembler.h"
#include "ptrace/BreakpointObserver.h"
#include "TaintedData.h"

namespace Aesalon {
namespace Monitor {

class TaintTracker : public PTrace::BreakpointObserver {
protected:
    typedef std::vector<Misc::SmartPointer<TaintedData> > tainted_data_t;
private:
    Misc::SmartPointer<ASM::Disassembler> disassembler;
    tainted_data_t tainted_data;
public:
    TaintTracker(Misc::SmartPointer<ASM::Disassembler> disassembler) : disassembler(disassembler) {}
    virtual ~TaintTracker() {}
    
    Misc::SmartPointer<TaintedData> get_tainted_data(Word address) const;
    void add_tainted_data(Misc::SmartPointer<TaintedData> new_tainted_data) {
        tainted_data.push_back(new_tainted_data);
    }
    
    virtual void handle_breakpoint(const PTrace::BreakpointReference &breakpoint);
    
    void place_breakpoints(std::string symbol);
};

} // namespace Monitor
} // namespace Aesalon

#endif
