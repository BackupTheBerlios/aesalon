#include <cstring>
#include <iostream>
#include <queue>

#include "ArgumentParser.h"

namespace Aesalon {
namespace Misc {

template<>ArgumentParser *Singleton<ArgumentParser>::instance = 0;

void ArgumentParser::parse_argv(char *argv[]) {
    std::size_t x = 0;
    bool end_found = false;
    while(argv[x] && argv[++x]) {
        /* Is it an argument? */
        if(argv[x][0] == '-' && !end_found) {
            if(argv[x][1] == '-') {
                if(argv[x][2] == 0) {
                    end_found = true; continue;
                }
                /* Try all the long-forms . . . */
                argument_map_t::iterator i = argument_map.begin();
                for(; i != argument_map.end(); i ++) {
                    bool handled = false;
                    switch((*i).second->get_type()) {
                    case Argument::BOOLEAN_ARGUMENT: {
                        BooleanArgument *ba = (*i).second.to<BooleanArgument>();
                        if(ba->get_enable_long_form() == argv[x]) ba->set_status(true), handled = true;
                        else if(ba->get_disable_long_form() == argv[x]) ba->set_status(false), handled = true;
                        if(!handled) std::cout << "Skipping over long-form argument \"" << ba->get_enable_long_form() << "\"/\""
                            << ba->get_disable_long_form() << "\", compared against \"" << argv[x] << "\"\n";
                        break;
                    }
                    case Argument::STRING_ARGUMENT: {
                        StringArgument *sa = (*i).second.to<StringArgument>();
                        if(sa->get_long_form() == argv[x]) sa->set_value(argv[++x]), handled = true;
                        break;
                    }
                    }
                    
                    if(handled) break;
                }
                
                if(i == argument_map.end()) {
                    throw UnknownArgumentException(argv[x]);
                }
            }
            else {
                std::queue<char> arguments;
                std::string argvstr = argv[x];
                std::string::iterator si = argvstr.begin();
                for(si++; si != argvstr.end(); si ++) arguments.push(*si);
                /* It's a bunch of short-form arguments, then. */
                argument_map_t::iterator i = argument_map.begin();
                while(!arguments.empty()) {
                    for(; i != argument_map.end(); i ++) {
                        if((*i).second->get_type() == Argument::BOOLEAN_ARGUMENT) {
                            if(arguments.front() == (*i).second.to<BooleanArgument>()->get_enable_short_form()) {
                                (*i).second.to<BooleanArgument>()->set_status(true);
                                break;
                            }
                            else if(arguments.front() == (*i).second.to<BooleanArgument>()->get_disable_short_form()) {
                                (*i).second.to<BooleanArgument>()->set_status(false);
                                break;
                            }
                        }
                    }
                    if(i == argument_map.end()) {
                        throw UnknownArgumentException(arguments.front());
                    }
                    arguments.pop();
                }
            }
        }
        /* Nope, it's a filename . . . */
        else add_file(new FileArgument(argv[x]));
    }
}

ArgumentParser::~ArgumentParser() {
    argument_map_t::iterator ai = argument_map.begin();
    for(; ai != argument_map.end(); ai ++) {
        delete (*ai).second;
    }
    file_argument_vector_t::iterator fi = file_argument_vector.begin();
    for(; fi != file_argument_vector.end(); fi ++) {
        delete (*fi);
    }
}

} // namespace Misc
} // namespace Aesalon
