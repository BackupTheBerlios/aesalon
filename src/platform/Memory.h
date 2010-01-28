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

/** Stores the memory associated with a program. */
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
    /** Generic constructor. */
    Memory() {}
    /** Virtual destructor. */
    virtual ~Memory() {}
    
    /** Returns the block at the specified MemoryAddress.
        @param address The address to look for.
        @return The @a MemoryBlock at @a address, or NULL if not found.
    */
    Misc::SmartPointer<MemoryBlock> get_block(MemoryAddress address) const;
    
    /** Handle an event as a "diff", and apply it to the current memory representation.
        @param event The event to handle.
    */
    void handle_event(Misc::SmartPointer<Event> event);
};

} // namespace Platform
} // namespace Aesalon

#endif
