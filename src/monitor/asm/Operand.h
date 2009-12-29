#ifndef AESALON_MONITOR_ASM_OPERAND_H
#define AESALON_MONITOR_ASM_OPERAND_H

#include "Types.h"
#include "Register.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Operand {
public:
    enum operand_type_e {
        REGISTER,
        DEREFERENCED_REGISTER_OFFSET,
        CONSTANT,
        DEREFERENCED_CONSTANT
    };
private:
    operand_type_e operand_type;
    Register reg;
    Word constant;
    SWord offset;
public:
    Operand(operand_type_e operand_type);
    virtual ~Operand();
    
    operand_type_e get_type() const { return operand_type; }
    void set_type(operand_type_e new_type) { operand_type = new_type; }
    
    const Register &get_register() const { return reg; }
    void set_register(Register new_register) { reg = new_register; }
    
    SWord get_offset() const { return offset; }
    void set_offset(const SWord new_offset) { offset = new_offset; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
