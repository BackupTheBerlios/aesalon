#include <cstring>

#include "ArgumentParser.h"

namespace Aesalon {

void ArgumentParser::parse_arguments(char *argv[]) {
    std::size_t x = 1;
    enum parse_mode_e {
        PARSE_MODE_NORMAL,
        PARSE_MODE_NOMORE
    } current_mode = PARSE_MODE_NORMAL;
    
    while(argv[x]) {
        if(current_mode == PARSE_MODE_NOMORE) {
            add_file_argument(argv[x++]);
            continue;
        }
        if(argv[0] == '-') {
        
        }
        add_file_argument(argv[x]);
        x ++;
    }
}


} // namespace Aesalon
