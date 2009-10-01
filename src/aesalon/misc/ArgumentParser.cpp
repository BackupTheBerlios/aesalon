#include <cstring>
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
        if(argv[x][0] == '-') {
            if(argv[x][1] == '-') {
                if(argv[x][2] == 0) {
                    end_found = true; continue;
                }
                /* Try all the long-forms . . . */
                argument_map_t::iterator i = argument_map.begin();
                for(; i != argument_map.end(); i ++) {
                    if((*i).first == argv[x]) {
                        switch((*i).second->get_type()) {
                            case Argument::BOOLEAN_ARGUMENT:
                                (*i).second.to<BooleanArgument>()->toggle_status();
                                break;
                            case Argument::STRING_ARGUMENT:
                                (*i).second.to<StringArgument>()->set_value(argv[++x]);
                                break;
                        }
                        break;
                    }
                }
                if(i == argument_map.end()) {
                    throw new UnknownArgumentException(argv[x]);
                }
            }
            else {
                std::queue<char> arguments;
                std::string argvstr = argv[x];
                std::string::iterator si = argvstr.begin();
                for(; si != argvstr.end(); si ++) arguments.push(*si);
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
                        throw new UnknownArgumentException(arguments.front());
                    }
                    arguments.pop();
                }
            }
        }
        /* No, it's a filename . . . ignore it for now. */
    }
}

} // namespace Misc
} // namespace Aesalon
