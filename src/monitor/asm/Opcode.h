#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
public:
    enum opcode_type_e {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        ADC,
        ADD,
        AND,
        BSF,
        BSR,
        BSWAP,
        BT,
        BTC,
        BTR,
        BTS,
        CALL,
        CALLF,
        CBW,
        CWDE,
        CDQE,
        CLC,
        CLD,
        CLFLUSH,
        CLI,
        CLTS,
        CMC,
        CMOVB,
        
#endif
    };
private:
    
public:
    Opcode() {}
    virtual ~Opcode() {}
    
    void parse(Misc::SmartPointer<Block> block);
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
