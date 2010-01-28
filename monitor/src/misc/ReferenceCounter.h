#ifndef AESALON_MISC_REFERENCE_COUNTER_H
#define AESALON_MISC_REFERENCE_COUNTER_H

#include <cstddef>
#include <map>

#include "Singleton.h"
#include "Exception.h"
#include "StreamAsString.h"

namespace Aesalon {
namespace Misc {

/** Exception thrown when asked to modify refcount on untracked memory. */
class UntrackedMemoryException : public Exception {
public:
    UntrackedMemoryException() : Exception(StreamAsString()
        << "Asked to decrement refcount on untracked block.") {}
};

/** Reference counter, used to reduce memory leaks from unfreed memory. */
class ReferenceCounter : public Singleton<ReferenceCounter> {
public:
    /** A simple refcount_t definition, simply a std::size_t. */
    typedef std::size_t refcount_t;
protected:
    /** A block of memory being tracked by the reference counter. */
    class MemoryBlock {
    private:
        /** The amount of references this block has. */
        refcount_t refcount;
    public:
        /** Get the number of references this block has.
            @return A refcount_t denoting the number of references.
        */
        refcount_t get_refcount() const { return refcount; }
        /** Set the number of references the current block has.
            @param new_refcount The new amount of references the block has.
        */
        void set_refcount(refcount_t new_refcount) { refcount = new_refcount; }
    };
    /** A type denoting a map of memory blocks, from void pointers to MemoryBlock instances. */
    typedef std::map<const void *, MemoryBlock> block_map_t;
private:
    /** The map of memory blocks. */
    block_map_t block_map;

public:
    /** Generic constructor, chains onto the Singleton constructor. */
    ReferenceCounter() : Singleton<ReferenceCounter>() {}
    /** Virtual destructor, doesn't do anything. */
    virtual ~ReferenceCounter() {}
    /** Increment the amount of references @a data has.
        @param data A pointer to increment the refcount of.
    */
    template<typename Type> void increment_block(const Type *data);
    /** Decrement the amount of references @a data has. Frees the memory if the refcount is decreased to zero.
        @param data A pointer to increment the refcount of.
    */
    template<typename Type> void decrement_block(const Type *data);
    
    /** Return the amount of references a given pointer has. */
    template<typename Type> refcount_t get_block_references(const Type *data);
};

template<typename Type>
void ReferenceCounter::increment_block(const Type *data) {
    if(data == NULL) return;
    
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) {
        MemoryBlock new_block;
        new_block.set_refcount(1);
        block_map[data] = new_block;
    }
    else {
        (*i).second.set_refcount((*i).second.get_refcount()+1);
    }
}

template<typename Type>
void ReferenceCounter::decrement_block(const Type *data) {
    if(data == NULL) return;
    
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) {
        /* This memory isn't currently in the counter . . . */
        return;
    }
    else {
        (*i).second.set_refcount((*i).second.get_refcount()-1);
    }
    if((*i).second.get_refcount() == 0) {
        delete data;
    }
}

template<typename Type>
ReferenceCounter::refcount_t ReferenceCounter::get_block_references(const Type* data) {
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) return 0;
    else return (*i).second.get_refcount();
}



} // namespace Misc
} // namespace Aesalon

#endif
