#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "Operand.h"
#include "StorageManager.h"

namespace ASM {

class Instruction {
private:
    StorageOffset storage_offset;
    bool memory_change;
    StorageOffset source;
    StorageOffset destination;
    
    Word address;
    
    void handle_opcode(std::string opcode, std::string operands);
public:
    Instruction(StorageOffset storage_offset, std::string instruction, Word address);
    virtual ~Instruction();
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    bool changes_memory() const { return memory_change; }
    
    StorageOffset get_source() const { return source; }
    StorageOffset get_destination() const { return destination; }
    
    Word get_address() const { return address; }
};

} // namespace ASM

#endif
