#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"
#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
private:
    Misc::SmartPointer<Block> opcode;
    std::vector<OperandType> expected_operands;
    std::string mnemonic;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    void parse_1byte_opcode();
    void parse_2byte_opcode();
#endif
public:
    Opcode(Misc::SmartPointer<Block> block);
    virtual ~Opcode() {}
    
    void parse();
    
    std::string get_mnemonic() const { return mnemonic; }
    
    std::vector<OperandType> get_expected_operands() const { return expected_operands; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
