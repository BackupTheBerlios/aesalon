#ifndef AESALON_TRACKER_SCOPE_MANAGER_H
#define AESALON_TRACKER_SCOPE_MANAGER_H

#include <vector>

#include "Scope.h"
#include "StorageOffset.h"
#include "elf/Parser.h"

namespace Tracker {

class ScopeManager {
private:
    std::vector<StorageOffset> scope_list;
public:
    ScopeManager();
    virtual ~ScopeManager();
    
    void add_scope(StorageOffset offset);
    void add_scopes(ELF::Parser *parser);
    StorageOffset get_scope_for(Word address);
    StorageOffset get_scope(ScopeID id);
};

} // namespace Tracker

#endif
