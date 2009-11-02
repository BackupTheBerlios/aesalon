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
    
    virtual Misc::SmartPointer<ParseSequence> get_data() = 0;
};

class StreamOutput : public String {
private:
    std::string data;
public:
    StreamOutput(std::string data) : String(String::STREAM_OUTPUT), data(data) {}
    virtual ~StreamOutput() {}
    
    std::string get_data() const { return data; }
    
    virtual Misc::SmartPointer<ParseSequence> get_data() { return NULL; }
};

class AsyncOutput : public String {
private:
    Misc::SmartPointer<ParseSequence> data;
public:
    AsyncOutput(Misc::SmartPointer<ParseSequence> data) : String(String::ASYNC_OUTPUT), data(data) {}
    virtual ~AsyncOutput() {}

    virtual Misc::SmartPointer<ParseSequence> get_data() { return data; }
};

class ResultRecord : public String {
private:
    Misc::SmartPointer<ParseSequence> data;
public:
    ResultRecord(Misc::SmartPointer<ParseSequence> data) : String(String::RESULT_RECORD), data(data) {}
    virtual ~ResultRecord() {}

    virtual Misc::SmartPointer<ParseSequence> get_data() { return data; }
};


} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
