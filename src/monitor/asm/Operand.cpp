#include <iostream>
#include "Operand.h"
#include "misc/String.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Operand::Operand(std::string operand_string) {
    Register reg = Register::from_string(operand_string);
    if(reg != Register::INVALID) {
        operand_type = REGISTER;
        this->reg = reg;
        return;
    }
    if(std::isdigit(operand_string[0])) {
        Misc::String::to<Word>(operand_string, address);
        return;
    }
    std::cout << "NOTE: Register offsets NYI." << std::endl;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
