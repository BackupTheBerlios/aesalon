#include <algorithm>
#include "ScopeManager.h"

namespace Tracker {

ScopeManager::ScopeManager() {

}

ScopeManager::~ScopeManager() {

}

void ScopeManager::add_scope(StorageOffset offset) {
    scope_list.push_back(offset);
}

void ScopeManager::add_scopes(ELF::Parser* parser) {
    
}

StorageOffset ScopeManager::get_scope_for(Word address) {
    std::lower_bound(scope_list.begin(), scope_list.end(), address);
    return -1;
}


} // namespace Tracker
