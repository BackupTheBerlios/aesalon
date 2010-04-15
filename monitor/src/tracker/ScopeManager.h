#ifndef AESALON_TRACKER_SCOPE_MANAGER_H
#define AESALON_TRACKER_SCOPE_MANAGER_H

#include <map>
#include <vector>

#include "Types.h"
#include "analyzer/Interface.h"
#include "ptrace/MapParser.h"

namespace Tracker {

class ScopeManager {
protected:
    typedef std::map<Word, Analyzer::Object> scope_map_t;
    typedef std::vector<Analyzer::Object> scope_vector_t;
private:
    scope_map_t scope_map;
    scope_vector_t scope_vector;
    
    PTrace::MapParser *map_parser;
public:
    ScopeManager();
    ~ScopeManager();
    
    Word32 get_scope_id(Word address);
};

} // namespace

#endif
