#ifndef AESALON_INTERFACE_GDB_STRING_H
#define AESALON_INTERFACE_GDB_STRING_H

#include "ParseData.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class NumberedString {
private:
    int number;
public:
    NumberedString(int number) : number(number) {}
    
    int get_number() const { return number; }
};

class String {
public:
    enum string_type_e {
        STREAM_OUTPUT, /* '~', '@', '&' */
        ASYNC_OUTPUT, /* '*' '+' '=' */
        RESULT_RECORD, /* '^' */
        TYPES
    };
private:
    string_type_e type;
    NumberedString *number;
protected:
    String(string_type_e type) : type(type), number(number) {}
public:
    virtual ~String() {}
    
    string_type_e get_type() const { return type; }
    
    virtual void set_number(NumberedString *new_number) { number = new_number; }
    virtual NumberedString *get_number() const { return number; }
    
    
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
