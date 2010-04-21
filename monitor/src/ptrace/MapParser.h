#ifndef AESALON_PTRACE_MAP_PARSER_H
#define AESALON_PTRACE_MAP_PARSER_H

#include <sys/types.h>

#include <vector>
#include <string>

#include "Types.h"
#include "analyzer/Interface.h"

namespace PTrace {

class MapParser {
protected:
    typedef std::vector<Analyzer::Object> object_vector_t;
private:
    pid_t pid;
    object_vector_t objects;
public:
    MapParser(pid_t pid);
    ~MapParser();
    
    const Analyzer::Object &get_object(Word address);
    std::string get_filename(Word address);
    
    void parse_maps();
};

} // namespace PTrace

#endif
