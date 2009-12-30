#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"
#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
private:
    std::string mnemonic;
public:
    Opcode();
    virtual ~Opcode() {}
    
    std::string get_mnemonic() const { return mnemonic; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
