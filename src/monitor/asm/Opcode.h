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
        MOV,
        ADD,
        SUB,
        MUL,
        DIV,
        INC,
        DEC
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
