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
    enum operand_size_e {
        SIZE_BYTE,
        SIZE_WORD,
        SIZE_DWORD,
        SIZE_QWORD
    };
private:
    operand_type_e operand_type;
    operand_size_e operand_size;
public:
    Operand(operand_type_e type, operand_size_e size) : operand_type(type), operand_size(size) {}
    virtual ~Operand() {}
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
