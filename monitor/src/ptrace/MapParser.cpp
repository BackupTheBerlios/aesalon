#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "MapParser.h"
#include "misc/StreamAsString.h"

namespace PTrace {

MapParser::MapParser(pid_t pid) : pid(pid) {
}

MapParser::~MapParser() {

}

const Analyzer::Object &MapParser::get_object(Word address) {
    static Analyzer::Object invalid("", 0, 0);
    for(object_vector_t::iterator i = objects.begin(); i != objects.end(); i ++) {
        if(i->get_address() <= address && (i->get_address() + i->get_size()) >= address) return *i;
    }
    parse_maps();
    for(object_vector_t::iterator i = objects.begin(); i != objects.end(); i ++) {
        if(i->get_address() <= address && (i->get_address() + i->get_size()) >= address) return *i;
    }
    std::cout << "Couldn't find 0x" << std::hex << address << " . . ." << std::endl;
    return invalid;
}

std::string MapParser::get_filename(Word address) {
    const Analyzer::Object &object = get_object(address);
    return object.get_name();
}

void MapParser::parse_maps() {
    std::string map_filename = Misc::StreamAsString() << "/proc/" << pid << "/maps";
    std::ifstream map_stream(map_filename.c_str());
    
    char buffer[1024];
    
    while(!map_stream.eof() && !map_stream.bad()) {
        map_stream.getline(buffer, sizeof(buffer));
        
        Word start_address, end_address;
        char mode[5];
        char filename[1024];
        
        if(std::sscanf(buffer, "%lx-%lx %s %s %s %s %s", &start_address, &end_address, mode, filename, filename, filename, filename) == -1) break;

        /* Only interested in the .text executable sections. */
        if(std::strncmp(mode, "r-xp", 4) != 0) continue;
        
        object_vector_t::iterator i = std::lower_bound(objects.begin(), objects.end(), start_address);
        Analyzer::Object object = Analyzer::Object(filename, start_address, end_address - start_address);
        if(i == objects.end()) objects.push_back(object);
        else if(i->get_address() != start_address) objects.insert(i, object);
    }
    
    map_stream.close();
}

} // namespace PTrace
