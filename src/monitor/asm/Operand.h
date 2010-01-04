#ifndef AESALON_MONITOR_ASM_OPERAND_H
#define AESALON_MONITOR_ASM_OPERAND_H

#include "Types.h"
#include "Register.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Operand {
public:
    enum operand_type_e {
        REGISTER,
        CONSTANT,
        DEREF_CONSTANT,
        REGISTER_OFFSET
    };
private:
    operand_type_e operand_type;
    Register::data_size_e operand_size;
public:
    Operand(operand_type_e type, Register::data_size_e size) : operand_type(type), operand_size(size) {}
    virtual ~Operand() {}
    
    operand_type_e get_type() const { return operand_type; }
    Register::data_size_e get_size() const { return operand_size; }
protected:
    void set_size(Register::data_size_e new_size) { operand_size = new_size; }
};

class RegisterOperand : public Operand {
private:
    Register which_register;
public:
    RegisterOperand(Register reg);
    virtual ~RegisterOperand() {}
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
