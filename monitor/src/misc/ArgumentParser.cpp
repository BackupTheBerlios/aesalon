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
    
    @file misc/ArgumentParser.cpp
*/

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <cstring>
#include <unistd.h>
#include <getopt.h>
#include "ArgumentParser.h"
#include "exception/ArgumentException.h"

namespace Misc {

ArgumentParser::ArgumentParser(char **argv) : argv(argv) {
    int x;
    for(x = 0; argv[x]; x ++) ;
    argc = x;
}

ArgumentParser::~ArgumentParser() {
    for(argument_map_t::iterator i = argument_map.begin(); i != argument_map.end(); i ++) {
        if(i->second) delete i->second;
    }
}

void ArgumentParser::parse() {
    struct option *option_list = new option[argument_map.size()+1];
    std::string option_string = "+";
    int found;
    
    std::size_t x = 0;
    for(argument_map_t::iterator i = argument_map.begin(); i != argument_map.end(); i ++, x ++) {
        option_list[x].name = i->second->get_long_form().c_str();
        option_list[x].has_arg = (int)i->second->get_argument_type();
        option_list[x].flag = &found;
        option_list[x].val = x;
        if(i->second->get_short_form()) option_string += i->second->get_short_form();
        switch(i->second->get_argument_type()) {
            case Argument::OPTIONAL_ARGUMENT:
                option_string += ':';
            case Argument::REQUIRED_ARGUMENT:
                option_string += ':';
            case Argument::NO_ARGUMENT:
                break;
        }
    }
    option_list[x].name = 0;
    option_list[x].has_arg = 0;
    option_list[x].flag = 0;
    option_list[x].val = 0;
    
    while(true) {
        int index = 0;
        found = getopt_long(argc, argv, option_string.c_str(), option_list, &index);
        if(found == -1) break;
        else if(found == 0) {
            Argument *arg = argument_map[option_list[index].name];
            arg->set_found(true);
            if(optarg) arg->set_data(optarg);
            else if(arg->get_argument_type() == Argument::REQUIRED_ARGUMENT)
                throw Exception::ArgumentException(Misc::StreamAsString() << "Option " << found << " expected argument");
            continue;
        }
        for(argument_map_t::iterator i = argument_map.begin(); i != argument_map.end(); i ++) {
            if(i->second->get_short_form() == found) {
                i->second->set_found(true);
                if(optarg) i->second->set_data(optarg);
                else if(i->second->get_argument_type() == Argument::REQUIRED_ARGUMENT) {
                    throw Exception::ArgumentException(Misc::StreamAsString() << "Option " << found << " expected argument");
                }
            }
        }
    }
    
    for(int x = optind; argv[x]; x ++) postargs.push_back(argv[x]);
    
    delete[] option_list;
}

} // namespace Misc
