#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
private:
    std::string opcode;
    bool change_memory;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    void handle_two_byte();
#endif
public:
    Opcode() : opcode(""), change_memory(false) {}
    virtual ~Opcode() {}
    
    void parse(Misc::SmartPointer<Block> block);
    std::string get_opcode() const { return opcode; }
    
    bool changes_memory() const { return change_memory; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
