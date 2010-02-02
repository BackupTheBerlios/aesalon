#include <iostream>
#include "Breakpoint.h"

namespace PTrace {

Breakpoint::Breakpoint(Word address, Byte original)
    : address(address), original(original), 
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
        BREAKPOINT_CHARACTER(0xcc),
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        BREAKPOINT_CHARACTER(0xcc),
#endif
    valid(true) {
    
    static std::size_t last_id = 0;
    id = ++last_id;
}

void Breakpoint::remove_observer(BreakpointObserver *observer) {
    for(observer_list_t::iterator i = observer_list.begin(); i != observer_list.end(); i ++) {
        if((*i) == observer) {
            observer_list.erase(i);
            break;
        }
    }
    if(observer_list.size() == 0) set_valid(false);
}

void Breakpoint::notify() {
    if(observer_list.empty()) {
        set_valid(false);
        return;
    }
    /*int last_size = observer_list.size();
    for(int x = 0; x < last_size && observer_list.size(); x ++) {
        observer_list_t::iterator i = observer_list.begin();
        for(int y = 0; y < x; y ++) i ++;
        if(*i) (*i)->handle_breakpoint(this);
        if((unsigned)last_size != observer_list.size()) x --;
    }*/
    /*std::cout << "observer_list.size(): " << observer_list.size() << std::endl;
    for(observer_list_t::iterator i = observer_list.begin(); !observer_list.empty() && i != observer_list.end(); i ++) {
        if(i == observer_list.end()) break;
        if((*i) != NULL) (*i)->handle_breakpoint(this);
    }*/
    for(std::size_t x = 0; x < observer_list.size() && observer_list.size();) {
        if(observer_list[x]->handle_breakpoint(this)) x ++;
    }
}

} // namespace PTrace
