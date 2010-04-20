#ifndef AESALON_PTRACE_MAP_PARSER_H
#define AESALON_PTRACE_MAP_PARSER_H

#include <sys/types.h>

#include <map>
#include <string>

#include "Types.h"
#include "analyzer/Interface.h"

namespace PTrace {

class MapParser {
private:
    pid_t pid;
    std::map<Word, Analyzer::Object> name_map;
public:
    MapParser(pid_t pid);
    ~MapParser();
    
    const Analyzer::Object &get_object(Word address);
    std::string get_filename(Word address) const;
private:
    void parse_map();
};

} // namespace PTrace

#endif
