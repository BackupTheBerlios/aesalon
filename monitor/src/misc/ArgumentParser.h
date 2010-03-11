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
    
    @file ArgumentParser.h
*/

#ifndef AESALON_ARGUMENT_PARSER_H
#define AESALON_ARGUMENT_PARSER_H

#include <string>
#include <vector>
#include <map>

#include "exception/ArgumentException.h"

namespace Misc {

class Argument {
public:
    enum argument_type_e {
        NO_ARGUMENT = 0,
        REQUIRED_ARGUMENT,
        OPTIONAL_ARGUMENT
    };
private:
    std::string long_form;
    char short_form;
    argument_type_e argument_type;
    std::string data;
    bool found;
public:
    Argument(std::string long_form, char short_form,
        argument_type_e argument_type, std::string default_data) : long_form(long_form),
        short_form(short_form), argument_type(argument_type), data(default_data), found(false) {}
    ~Argument() {}
    
    std::string get_long_form() const { return long_form; }
    char get_short_form() const { return short_form; }
    std::string get_data() const { return data; }
    void set_data(std::string new_data) { data = new_data; }
    argument_type_e get_argument_type() const { return argument_type; }
    bool is_found() const { return found; }
    void set_found(bool new_found) { found = new_found; }
};

class ArgumentParser {
protected:
    typedef std::map<std::string, Argument *> argument_map_t;
    typedef std::vector<std::string> postargs_t;
private:
    int argc;
    char **argv;
    argument_map_t argument_map;
    postargs_t postargs;
public:
    ArgumentParser(char **argv);
    ~ArgumentParser();
    
    /** Adds an argument to the argument list. 
        @param argument The argument to add. It is not the responsibility of the caller to free this.
    */
    void add_argument(Argument *argument) {
        argument_map[argument->get_long_form()] = argument;
    }
    
    Argument *get_argument(std::string long_form) {
        Argument *arg = argument_map[long_form];
        if(arg) return arg;
        throw Exception::ArgumentException(Misc::StreamAsString() << "Attempt to retreieve non-existent argument \"" << long_form << "\"");
    }
    
    std::size_t get_postargs() const { return postargs.size(); }
    std::string get_postarg(std::size_t which) { return postargs[which]; }
    
    void parse(); 
};

} // namespace Misc

#endif
