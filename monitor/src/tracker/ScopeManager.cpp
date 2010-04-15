#include <algorithm>

#include "ScopeManager.h"
#include "Initializer.h"

namespace Tracker {

ScopeManager::ScopeManager() {
    map_parser = Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser();
}

ScopeManager::~ScopeManager() {
    
}

Word32 ScopeManager::get_scope_id(Word address) {
    
    return 0;
}

} // namespace Tracker
