#ifndef AESALON_MONITOR_TAINTED_DATA_H
#define AESALON_MONITOR_TAINTED_DATA_H

#include "Types.h"
#include "asm/Register.h"

namespace Aesalon {
namespace Monitor {

class TaintedData {
private:
    Word address;
    ASM::Register::data_size_e size;
public:
    TaintedData(Word address, ASM::Register::data_size_e size) : address(address), size(size) {}
    virtual ~TaintedData() {}
    
    Word get_address() const { return address; }
    ASM::Register::data_size_e get_size() const { return size; }
};

} // namespace Monitor
} // namespace Aesalon

#endif
