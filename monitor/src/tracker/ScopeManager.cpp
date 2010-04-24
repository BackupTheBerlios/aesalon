#include <algorithm>
#include <iostream>

#include "ScopeManager.h"
#include "Initializer.h"
#include "event/ScopeEvent.h"

namespace Tracker {

Scope::Scope(const std::string &name, Word address, Word size, Word32 id) : Object(name, address, size), id(id) {
    
}

ScopeManager::ScopeManager() {
    interface = Initializer::get_instance()->get_analyzer_interface();
    last_id = 0;
    map_parser = NULL;
}

ScopeManager::~ScopeManager() {
    
}

Word32 ScopeManager::get_scope_id(Word address) {
    
    return 0;
}

Event::ScopeEvent *ScopeManager::get_scope(Word address, Word32 &id) {
    id = 0;
    
    scope_vector_t::iterator iterator;
    int size = scope_vector.size();
    int lower_range = 0, upper_range = size-1;
    
    scope_vector_t::iterator begin = scope_vector.begin();
    iterator = begin;
    
    while(size) {
        iterator = begin + lower_range + (size / 2);
        if((*iterator) > address) {
            size /= 2;
            lower_range += size;
        }
        else if((*iterator) < address) {
            size /= 2;
            upper_range -= size;
        }
        else break;
    }
    
    if(scope_vector.size() && iterator != scope_vector.end() && iterator->get_address() <= address && (iterator->get_address() + iterator->get_size()) >= address) {
        id = iterator->get_id();
        std::cout << "ScopeManager: found properly, id is " << id << std::endl;
        return NULL;
    }
    
    if(map_parser == NULL) {
        map_parser = Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser();
    }
    
    Analyzer::Object file_object = map_parser->get_object(address);
    Analyzer::File *file = interface->get_file(file_object.get_name());
    if(!file) {
        file = interface->parse_file(file_object.get_name());
        if(!file) {
            std::cout << "ScopeManager: couldn't parse file to get information" << std::endl;
            return NULL;
        }
    }
    
    if(file != interface->get_file()) address -= file_object.get_address();
    Analyzer::Object symbol = file->get_symbol_for(address);
    
    if(symbol.get_address() == 0 && symbol.get_size() == 0) {
        std::cout << "ScopeManager: NULL object given for address " << std::hex << address << std::endl;
        return NULL;
    }
    
    Scope scope(symbol.get_name(), symbol.get_address(), symbol.get_size(), ++last_id);
    id = last_id;
    
    
    iterator = begin;
    while(size) {
        iterator = begin + lower_range + (size / 2);
        if((*iterator) > address) {
            size /= 2;
            lower_range += size;
        }
        else if((*iterator) < address) {
            size /= 2;
            upper_range -= size;
        }
        else break;
    }
    
    scope_vector.insert(iterator, scope);
    
    std::cout << "ScopeManager: returning new ScopeEvent" << std::endl;
    
    return new Event::ScopeEvent(Event::ScopeEvent::NEW_SCOPE, scope.get_id(), scope.get_name());
}

} // namespace Tracker
