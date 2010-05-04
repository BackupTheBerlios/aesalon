#include <algorithm>
#include <iostream>

#include "ScopeManager.h"
#include "Initializer.h"
#include "event/ScopeEvent.h"
#include "exception/AnalyzerException.h"
#include "misc/Message.h"

namespace Tracker {

Scope::Scope(const std::string &name, Word address, Word size, Word32 id) : Object(name, address, size), id(id) {
    
}

ScopeManager::ScopeManager() {
    interface = Initializer::get_instance()->get_analyzer_interface();
    last_id = 0;
    map_parser = NULL;
    
    full_backtrace = !Initializer::get_instance()->get_argument_parser()->get_argument("no-backtrace")->is_found();
    platform_bits = Initializer::get_instance()->get_analyzer_interface()->get_file()->get_attribute("platform_bits");
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
    
    if(scope_vector.size() && iterator != scope_vector.end() && iterator->get_address() <= address
        && (iterator->get_address() + iterator->get_size()) >= address) {
        
        id = iterator->get_id();
        return NULL;
    }
    
    if(map_parser == NULL) {
        map_parser = Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser();
    }
    
    Analyzer::Object file_object = map_parser->get_object(address);
    Analyzer::File *file = interface->get_file(file_object.get_name());
    if(!file) {
        try {
            file = interface->parse_file(file_object.get_name());
        }
        catch(Exception::AnalyzerException exception) {
            Misc::Message(Misc::Message::WARNING_MESSAGE,
                Misc::StreamAsString() << "Couldn't open file referenced by process map: \"" << file_object.get_name() << "\"");
            return NULL;
        }
    }
    
    if(file != interface->get_file()) address -= file_object.get_address();
    Analyzer::Object symbol = file->get_symbol_for(address);
    
    if(symbol.get_address() == 0 && symbol.get_size() == 0) {
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
    
    return new Event::ScopeEvent(Event::ScopeEvent::NEW_SCOPE, scope.get_id(), scope.get_name());
}

void ScopeManager::push_scope(Block *block, Word *scope, Word scope_size) {
    Word x = 0;
    
    if(scope_size > 65535) scope_size = 65535;
    if(!full_backtrace) block->push_word(1, 16);
    else block->push_word(scope_size, 16);
    
    if(scope == NULL) return;
    
    do {
        push_scope(block, scope[x]);
    } while(++x < scope_size && full_backtrace);
}

void ScopeManager::push_scope(Block *block, Word address) {
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
    
    if(scope_vector.size() && iterator != scope_vector.end() && iterator->get_address() <= address
        && (iterator->get_address() + iterator->get_size()) >= address) {
        
        block->push_word(iterator->get_id(), 32);
        return;
    }
    
    if(map_parser == NULL) {
        map_parser = Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser();
    }
    
    Analyzer::Object file_object = map_parser->get_object(address);
    Analyzer::File *file = interface->get_file(file_object.get_name());
    if(!file) {
        try {
            file = interface->parse_file(file_object.get_name());
        }
        catch(Exception::AnalyzerException exception) {
            Misc::Message(Misc::Message::WARNING_MESSAGE,
                Misc::StreamAsString() << "Couldn't open file referenced by process map: \"" << file_object.get_name() << "\"");
            block->push_word(0, 32);
            return;
        }
    }
    
    if(file != interface->get_file()) {
        address -= file_object.get_address();
    }
    Analyzer::Object symbol = file->get_symbol_for(address);
    
    if(symbol.get_address() == 0 && symbol.get_size() == 0) {
        block->push_word(0, 32);
        return;
    }
    
    Scope scope(symbol.get_name(), symbol.get_address(), symbol.get_size(), ++last_id);
    
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
    
    block->push_word(last_id, 32);
    
    Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    Block *serialized = Event::ScopeEvent(Event::ScopeEvent::NEW_SCOPE, scope.get_id(), scope.get_name()).serialize(platform_bits);
    block->prepend(serialized);
    delete serialized;
}

} // namespace Tracker
