#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
private:
    Misc::SmartPointer<Block> opcode;
public:
    Opcode(Misc::SmartPointer<Block> block);
    virtual ~Opcode() {}
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
