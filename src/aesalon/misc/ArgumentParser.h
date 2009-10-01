#ifndef AESALON_MISC_ARGUMENT_PARSER_H
#define AESALON_MISC_ARGUMENT_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "Singleton.h"
#include "SmartPointer.h"
#include "Exception.h"
#include "StreamAsString.h"

namespace Aesalon {
namespace Misc {

class UnknownArgumentException : public Exception {
public:
    UnknownArgumentException(std::string what) : Exception(StreamAsString()
        << "Unknown argument encountered: \"" << what << "\"") {}
    UnknownArgumentException(char what) : Exception(StreamAsString()
        << "Unknown short-form argument: \'" << what << "\'") {}
};

/* The Argument class needs to encapsulate two very different jobs.
    The first, and most obvious, is a simple boolean toggle switch, such as
        '--enable-debug-information'/'--disable-debug-information'; with shortened
        versions like -E/-d, that have a default value.
    The second, and second-most-obvious, is a string argument, something along
        the lines of '--library-path ./libaesalon_overload.so'/
        '-L ./libaesalon_overload.so' -- these may also have a default value.
*/

class BooleanArgument;
class StringArgument;

class Argument {
public:
    enum argument_type_e {
        BOOLEAN_ARGUMENT,
        STRING_ARGUMENT
    };
private:
    argument_type_e type;
public:
    Argument(argument_type_e type) : type(type) {}
    virtual ~Argument() {}
    
    argument_type_e get_type() const { return type; }
};

class BooleanArgument : public Argument {
private:
    std::string enable_long_form;
    char enable_short_form;
    std::string disable_long_form;
    char disable_short_form;
    bool status;
public:
    BooleanArgument(std::string enable_long_form, char enable_short_form,
        std::string disable_long_form, char disable_short_form,
        bool default_value = false) : Argument(Argument::BOOLEAN_ARGUMENT),
        enable_long_form(enable_long_form),
        enable_short_form(enable_short_form),
        disable_long_form(disable_long_form),
        disable_short_form(disable_short_form),
        status(default_value) {}
    
    std::string get_enable_long_form() const { return enable_long_form; }
    char get_enable_short_form() const { return enable_short_form; }
    std::string get_disable_long_form() const { return disable_long_form; }
    char get_disable_short_form() const { return disable_short_form; }
    void set_status(bool new_status) { status = new_status; }
    void toggle_status() { status = !status; }
    bool get_status() const { return status; }
};

class StringArgument : public Argument {
private:
    std::string long_form;
    char short_form;
    std::string value;
public:
    StringArgument(std::string long_form, char short_form,
        std::string default_value = "") : Argument(Argument::STRING_ARGUMENT),
        long_form(long_form), short_form(short_form), value(default_value) {}
    
    std::string get_long_form() const { return long_form; }
    char get_short_form() const { return short_form; }
    
    void set_value(std::string new_value) { value = new_value; }
    std::string get_value() const { return value;}
};

class ArgumentParser : public Misc::Singleton<ArgumentParser> {
public:
    typedef std::map<std::string, SmartPointer<Argument> > argument_map_t;
    typedef std::vector<std::string> filename_map_t;
private:
    argument_map_t argument_map;
public:
    ArgumentParser() : Singleton<ArgumentParser>() {}
    virtual ~ArgumentParser() {}
    
    void add_argument(std::string reference_name, SmartPointer<Argument> argument) {
        argument_map[reference_name] = argument;
    }
    SmartPointer<Argument> get_argument(std::string reference_name) {
        return argument_map[reference_name];
    }
    
    void parse_argv(char *argv[]);
};

} // namespace Misc
} // namespace Aesalon

#endif
