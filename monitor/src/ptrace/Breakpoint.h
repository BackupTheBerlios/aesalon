#ifndef AESALON_MONITOR_BREAKPOINT_H
#define AESALON_MONITOR_BREAKPOINT_H

#include <vector>

#include "Types.h"
#include "BreakpointObserver.h"

namespace PTrace {

class Breakpoint {
protected:
    typedef std::vector<BreakpointObserver *> observer_list_t;
private:
    Word address;
    Byte original;
    const Byte BREAKPOINT_CHARACTER;
    std::size_t id;
    bool valid;
    
    observer_list_t observer_list;
public:
    Breakpoint(Word address, Byte original);
    virtual ~Breakpoint() {}
    
    std::size_t get_id() const { return id; }
    Byte get_original() const { return original; }
    Word get_address() const { return address; }
    Byte get_breakpoint_character() const { return BREAKPOINT_CHARACTER; }
    
    bool is_valid() const { return valid; }
    void set_valid(bool new_validity) { valid = new_validity; }
    
    void add_observer(BreakpointObserver *observer)
        { observer_list.push_back(observer); set_valid(true); }
    void remove_observer(BreakpointObserver *observer);
    
    void notify();
};

} // namespace PTrace

#endif
