#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Opcode::Opcode(Misc::SmartPointer<Block> block) {
    std::size_t opcode_size = 0;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(*block->get_data(0) != 0x0f) opcode_size = 1;
    else opcode_size = 2;
#endif
    opcode = block->subset(0, opcode_size);
    block->remove(0, opcode_size);
}

void Opcode::parse() {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(opcode->get_size() == 1) parse_1byte_opcode();
    else if(opcode->get_size() == 2) parse_2byte_opcode();
#endif
}

#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
void Opcode::parse_1byte_opcode() {
    switch(*opcode->get_data(0)) {
        case 0x00:
            mnemonic = "add";
            expected_operands.push_back(OperandType(OperandType::REGISTER, OperandType::BYTE));
            expected_operands.push_back(OperandType(OperandType::REGISTER, OperandType::BYTE));
            break;
        case 0x01:
            mnemonic = "add";
            expected_operands.push_back(OperandType(OperandType::REGISTER, OperandType::WORD | OperandType::DWORD | OperandType::QWORD));
            expected_operands.push_back(OperandType(OperandType::REGISTER, OperandType::WORD | OperandType::DWORD | OperandType::QWORD));
            break;
    }
    /*expected_operands.push_back(OperandType(Aesalon::Monitor::ASM::OperandType::REGISTER, 8));*/
}

void Opcode::parse_2byte_opcode() {
    
}
#endif

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
