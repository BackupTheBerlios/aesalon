#include <algorithm>

#include "ScopeManager.h"
#include "Initializer.h"
#include "event/ScopeEvent.h"

namespace Tracker {

Scope::Scope(const std::string &name, Word address, Word size, Word32 id) : Object(name, address, size), id(id) {
    
}

bool Scope::operator<(const Tracker::Scope &other) const {
    return get_address() < other.get_address();
}

bool Scope::operator<(Word address) const {
    return get_address() < address;
}


ScopeManager::ScopeManager() {
    interface = Initializer::get_instance()->get_analyzer_interface();
    last_id = 0;
}

ScopeManager::~ScopeManager() {
    
}

Word32 ScopeManager::get_scope_id(Word address) {
    
    return 0;
}

Event::ScopeEvent *ScopeManager::get_scope(Word address, Word32 &id) {
    id = 0;
    scope_vector_t::iterator iterator = std::lower_bound(scope_vector.begin(), scope_vector.end(), address);
    if(iterator != scope_vector.end() && iterator->get_address() <= address && (iterator->get_address() + iterator->get_size()) >= address) {
        id = iterator->get_id();
        return NULL;
    }
    
    if(map_parser == NULL) {
        map_parser = Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser();
    }
    std::string filename = map_parser->get_filename(address);
    Analyzer::File *file = interface->get_file(filename);
    if(!file) {
        file = interface->parse_file(filename);
        if(!file) return NULL;
    }
    
    Analyzer::Object symbol = file->get_symbol_for(address);
    
    if(symbol.get_address() == 0 && symbol.get_size() == 0) return NULL;
    
    Scope scope(symbol.get_name(), symbol.get_address(), symbol.get_size(), ++last_id);
    
    scope_vector.insert(iterator, scope);
    
    return new Event::ScopeEvent(Event::ScopeEvent::NEW_SCOPE, scope.get_id(), scope.get_name());
}

} // namespace Tracker
