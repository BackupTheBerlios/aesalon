#ifndef AESALON_PLATFORM_MEMORY_H
#define AESALON_PLATFORM_MEMORY_H

#include <string>
#include <set>

#include "MemoryAddress.h"
#include "MemoryBlock.h"
#include "Event.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Platform {

class Memory {
protected:
    typedef std::set<Misc::SmartPointer<MemoryBlock> > block_set_t;
private:
    block_set_t block_set;
    
    void add_block(Misc::SmartPointer<MemoryBlock> new_block) {
        block_set.insert(new_block);
    }
    void remove_block(Misc::SmartPointer<MemoryBlock> block);
public:
    Memory() {}
    virtual ~Memory() {}
    
    Misc::SmartPointer<MemoryBlock> get_block(MemoryAddress address) const;
    
    void handle_event(Misc::SmartPointer<Event> event);
};

} // namespace Platform
} // namespace Aesalon

#endif
