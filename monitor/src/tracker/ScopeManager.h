#ifndef AESALON_TRACKER_SCOPE_MANAGER_H
#define AESALON_TRACKER_SCOPE_MANAGER_H

#include <map>
#include <vector>

#include "Types.h"
#include "analyzer/Interface.h"
#include "ptrace/MapParser.h"

namespace Event {
class ScopeEvent;
} // namespace Event

namespace Tracker {

class Scope : public Analyzer::Object {
private:
    Word32 id;
public:
    Scope(const std::string &name, Word address, Word size, Word32 id);
    
    Word32 get_id() const { return id; }
};

class ScopeManager {
protected:
    typedef std::vector<Scope> scope_vector_t;
private:
    scope_vector_t scope_vector;
    
    Word32 last_id;
    
    PTrace::MapParser *map_parser;
    Analyzer::Interface *interface;
public:
    ScopeManager();
    ~ScopeManager();
    
    Word32 get_scope_id(Word address);
    Event::ScopeEvent *get_scope(Word address, Word32 &id);
};

} // namespace Tracker

#endif
