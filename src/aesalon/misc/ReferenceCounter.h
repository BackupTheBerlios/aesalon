#ifndef AESALON_MISC_REFERENCE_COUNTER_H
#define AESALON_MISC_REFERENCE_COUNTER_H

#include <cstddef>
#include <map>

#include "Singleton.h"
#include "Exception.h"
#include "StreamAsString.h"

namespace Aesalon {
namespace Misc {

class UntrackedMemoryException : public Exception {
public:
    UntrackedMemoryException() : Exception(StreamAsString()
        << "Asked to decrement refcount on untracked block.") {}
};

class ReferenceCounter : public Singleton<ReferenceCounter> {
public:
    typedef std::size_t refcount_t;
protected:
    class MemoryBlock {
    private:
        refcount_t refcount;
    public:
        refcount_t get_refcount() const { return refcount; }
        void set_refcount(refcount_t new_refcount) { refcount = new_refcount; }
    };
    typedef std::map<const void *, MemoryBlock> block_map_t;
private:
    block_map_t block_map;

public:
    ReferenceCounter() {}
    virtual ~ReferenceCounter() {}
    template<typename Type> void increment_block(const Type *data);
    template<typename Type> void decrement_block(const Type *data);
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
