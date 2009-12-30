#ifndef AESALON_MONITOR_ASM_OPERAND_H
#define AESALON_MONITOR_ASM_OPERAND_H

#include "Types.h"
#include "Register.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class OperandType {
public:
    enum {
        REGISTER = 0x01,
        REGISTER_OFFSET = 0x02,
        CONSTANT = 0x04,
        DEREF_CONSTANT = 0x08
    };
    enum {
        BYTE = 0x01,
        WORD = 0x02,
        DWORD = 0x04,
        QWORD = 0x08
    };
private:
    std::size_t operand_type;
    std::size_t data_size;
public:
    OperandType(std::size_t operand_type = 0, std::size_t data_size = 0) :
        operand_type(operand_type), data_size(data_size) {}
    
    std::size_t get_type() const { return operand_type; }
    void set_type(std::size_t new_type) { operand_type = new_type; }
    std::size_t get_data_size() const { return data_size; }
    void set_data_size(std::size_t new_size) { data_size = new_size; }
};

class Operand {
private:
    OperandType operand_type;
    Register reg;
    Word constant;
    SWord offset;
public:
    Operand(OperandType type, Misc::SmartPointer<Block> block);
    virtual ~Operand() {}
    
    const OperandType &get_type() const { return operand_type; }
    
    const Register &get_register() const { return reg; }
    Word get_constant() const { return constant; }
    SWord get_offset() const { return offset; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
