/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file asm/Register.cpp
*/

#include <map>
#include <iostream>
#include "Register.h"

namespace ASM {

Register::data_size_e Register::get_register_size() const {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM_x86_64
    switch(reg) {
        case AL:
        case AH:
        case BL:
        case BH:
        case CL:
        case CH:
        case DL:
        case DH:
            return SIZE_BYTE;
        case AX:
        case BX:
        case CX:
        case DX:
            return SIZE_WORD;
        case EAX:
        case EBX:
        case ECX:
        case EDX:
        case EDI:
        case ESI:
        case EBP:
        case ESP:
            return SIZE_DWORD;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RAX:
        case RBX:
        case RCX:
        case RDX:
        case R8:
        case R9:
        case R10:
        case R11:
        case R12:
        case R13:
        case R14:
        case R15:
        case RSI:
        case RDI:
        case RSP:
        case RBP:
        case RIP:
            return SIZE_QWORD;
        default:
            return SIZE_BYTE;
#endif
    }
#endif
    return SIZE_BYTE;
}

Register Register::from_string(std::string string) {
    std::map<std::string, register_e> reg_map;
    
    reg_map["al"] = AL;
    reg_map["ah"] = AH;
    reg_map["bl"] = BL;
    reg_map["bh"] = BH;
    reg_map["cl"] = CL;
    reg_map["ch"] = CH;
    reg_map["dl"] = DL;
    reg_map["dh"] = DH;
    reg_map["ax"] = AX;
    reg_map["bx"] = BX;
    reg_map["cx"] = CX;
    reg_map["dx"] = DX;
    reg_map["eax"] = EAX;
    reg_map["ebx"] = EBX;
    reg_map["ecx"] = ECX;
    reg_map["edx"] = EDX;
    reg_map["esi"] = ESI;
    reg_map["edi"] = EDI;
    reg_map["esp"] = ESP;
    reg_map["ebp"] = EBP;
    reg_map["eip"] = EIP;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    reg_map["rax"] = RAX;
    reg_map["rbx"] = RBX;
    reg_map["rcx"] = RCX;
    reg_map["rdx"] = RDX;
    reg_map["r8"] = R8;
    reg_map["r9"] = R9;
    reg_map["r10"] = R10;
    reg_map["r11"] = R11;
    reg_map["r12"] = R12;
    reg_map["r13"] = R13;
    reg_map["r14"] = R14;
    reg_map["r15"] = R15;
    reg_map["rsi"] = RSI;
    reg_map["rdi"] = RDI;
    reg_map["rsp"] = RSP;
    reg_map["rbp"] = RBP;
    reg_map["rip"] = RIP;
#endif
    reg_map["cs"] = CS;
    reg_map["ss"] = SS;
    
    for(std::string::iterator i = string.begin(); i != string.end(); i ++) {
        (*i) = std::tolower(*i);
    }
    
    return reg_map[string];
}

} // namespace ASM
