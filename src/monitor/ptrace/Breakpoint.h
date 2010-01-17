#ifndef AESALON_MONITOR_BREAKPOINT_H
#define AESALON_MONITOR_BREAKPOINT_H

#include <set>

#include "Types.h"
#include "platform/MemoryAddress.h"
#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class Breakpoint {
protected:
    typedef std::set<Misc::SmartPointer<BreakpointObserver> > observer_list_t;
private:
    Platform::MemoryAddress address;
    Byte original;
    const Byte BREAKPOINT_CHARACTER;
    std::size_t id;
    bool valid;
    
    observer_list_t observer_list;
public:
    Breakpoint(Platform::MemoryAddress address, Byte original);
    virtual ~Breakpoint() {}
    
    std::size_t get_id() const { return id; }
    Byte get_original() const { return original; }
    Platform::MemoryAddress get_address() const { return address; }
    Byte get_breakpoint_character() const { return BREAKPOINT_CHARACTER; }
    
    bool is_valid() const { return valid; }
    void set_valid(bool new_validity) { valid = new_validity; }
    
    void add_observer(Misc::SmartPointer<BreakpointObserver> observer)
        { observer_list.insert(observer); }
    void remove_observer(Misc::SmartPointer<BreakpointObserver> observer) {
        observer_list_t::iterator i = observer_list.find(observer);
        if(i != observer_list.end()) observer_list.erase(i);
    }
    
    void notify();
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
