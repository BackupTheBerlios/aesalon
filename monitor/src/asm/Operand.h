#ifndef AESALON_MONITOR_ASM_OPERAND_H
#define AESALON_MONITOR_ASM_OPERAND_H

#include "Types.h"
#include "Register.h"
#include "StorageOffset.h"

namespace ASM {

class Operand {
public:
    enum operand_type_e {
        REGISTER,
        MEMORY_ADDRESS,
        REGISTER_OFFSET
    };
private:
    StorageOffset storage_offset;
    operand_type_e operand_type;
    Word address;
    SWord offset;
    Register reg;
    Register::data_size_e data_size;
public:
    Operand(StorageOffset storage_offset, std::string operand_string);
    Operand(StorageOffset storage_offset, operand_type_e operand_type, Word address = 0,
        Register reg = Register::INVALID, SWord offset = 0) : storage_offset(storage_offset),
        operand_type(operand_type), address(address), offset(offset), reg(reg) {}
    virtual ~Operand() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    Word get_address() const { return address; }
    void set_address(Word new_address) { address = new_address; }
    
    Register get_register() const { return reg; }
    void set_register(Register new_register) { reg = new_register; }
};

} // namespace ASM

#endif
