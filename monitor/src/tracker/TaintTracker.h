#ifndef AESALON_MONITOR_TAINT_TRACKER_H
#define AESALON_MONITOR_TAINT_TRACKER_H

#include <vector>

#include "asm/Instruction.h"
#include "asm/InstructionList.h"
#include "asm/Disassembler.h"
#include "ptrace/BreakpointObserver.h"
#include "TaintedData.h"




class TaintTracker : public PTrace::BreakpointObserver {
protected:
    typedef std::vector<TaintedData *> tainted_data_t;
private:
    ASM::Disassembler *disassembler;
    tainted_data_t tainted_data;
public:
    TaintTracker(ASM::Disassembler *disassembler) : disassembler(disassembler) {}
    virtual ~TaintTracker() {}
    
    TaintedData *get_tainted_data(Word address) const;
    void add_tainted_data(TaintedData *new_tainted_data) {
        tainted_data.push_back(new_tainted_data);
    }
    
    virtual void handle_breakpoint(const PTrace::BreakpointReference &breakpoint);
    
    void place_breakpoints(std::string symbol);
};




#endif
