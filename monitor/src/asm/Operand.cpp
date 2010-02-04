#include <iostream>
#include "Operand.h"
#include "misc/String.h"

namespace ASM {

Operand::Operand(StorageOffset storage_offset, std::string operand_string) : storage_offset(storage_offset) {
    Register reg = Register::from_string(operand_string);
    if(reg != Register::INVALID) {
        operand_type = REGISTER;
        this->reg = reg;
        data_size = reg.get_register_size();
        return;
    }
    if(std::isdigit(operand_string[0])) {
        Misc::String::to<Word>(operand_string, address);
        operand_type = MEMORY_ADDRESS;
        return;
    }
    operand_type = REGISTER_OFFSET;
    /* Some examples:
        DWORD PTR [rbp-0x14]
        QWORD PTR [rip+0x200541]
    */
    if(Misc::String::begins_with(operand_string, "QWORD")) data_size = Register::SIZE_QWORD;
    else if(Misc::String::begins_with(operand_string, "DWORD")) data_size = Register::SIZE_DWORD;
    else if(Misc::String::begins_with(operand_string, "WORD")) data_size = Register::SIZE_WORD;
    else if(Misc::String::begins_with(operand_string, "BYTE")) data_size = Register::SIZE_BYTE;
    operand_string.erase(0, operand_string.find("[")+1);
    std::string offset;
    if(operand_string.find("+") != std::string::npos) {
        offset = operand_string.substr(operand_string.find("+"));
        operand_string.erase(operand_string.find("+"));
    }
    else if(operand_string.find("-") != std::string::npos) {
        offset = operand_string.substr(operand_string.find("-"));
        operand_string.erase(operand_string.find("-"));
    }
    Misc::String::to<SWord>(offset, this->offset);
    this->reg = Register::from_string(operand_string);
}

} // namespace ASM