#include <fstream>
#include <cstring>

#include "MapParser.h"
#include "misc/StreamAsString.h"

namespace PTrace {

MapParser::MapParser(pid_t pid) : pid(pid) {

}

MapParser::~MapParser() {

}

/*long unsigned int get_libc_offset(char *libc_path) {
    char buffer[1024];
    
    sprintf(buffer, "/proc/%i/maps", getpid());
    
    int fd = open(buffer, O_RDONLY);
    unsigned long address = 0;
    
    int ret = 1;
    while(ret > 0) {
        char c = 0;
        int pos = 0;
        while(c != '\n' && (ret = read(fd, &c, sizeof(c)))) buffer[pos++] = c;
        buffer[pos] = 0;
        
        char str[128];
        sscanf(buffer, "%lx-%*lx %*s %*s %*s %*s %s", &address, str, str, str, str, str, str);

        if(strcmp(str, libc_path) == 0) {
            break;
        }
    }
    
    close(fd);
    
    return address;
}*/

std::string MapParser::get_filename(Word address) const {
    /* TODO: implement cached lookup . . . */
    
    
    
    std::string map_filename = Misc::StreamAsString() << "/proc/" << pid << "/maps";
    std::ifstream map_stream(map_filename.c_str());
    
    char buffer[1024];
    
    while(!map_stream.eof() && !map_stream.bad()) {
        map_stream.getline(buffer, sizeof(buffer));
        
        Word start_address, end_address;
        char mode[5];
        char filename[1024];
        
        std::sscanf(buffer, "%lx-%lx %s %s %s %s %s", &start_address, &end_address, mode, filename, filename, filename, filename);
        
        /* Only interested in the .text executable sections. */
        if(std::strncmp(mode, "r-xp", 4) != 0) continue;
        
        /* TODO: store file in cache . . . */
        
        /* Check the address . . . */
        if(address >= start_address && address <= end_address) {
            return filename;
        }
    }
    
    map_stream.close();
    
    return "";
}

} // namespace PTrace
