#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

RegisterOperand::RegisterOperand(Register reg) : Operand(Operand::REGISTER, reg.get_register_size()), which_register(reg) {
    
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
