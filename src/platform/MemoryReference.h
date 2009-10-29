#ifndef AESALON_PLATFORM_MEMORY_REFERENCE_H
#define AESALON_PLATFORM_MEMORY_REFERENCE_H

#include <string>

#include "MemoryBlock.h"
#include "misc/SmartPointer.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

typedef std::size_t MemoryReferenceID;

class MemoryReferenceScope {
private:
    Misc::SmartPointer<MemoryBlock> storage_block;
    std::string variable_name;
public:
    MemoryReferenceScope() {}
    virtual ~MemoryReferenceScope() {}
    
    Misc::SmartPointer<MemoryBlock> get_storage_block() const { return storage_block; }
    void set_storage_block(Misc::SmartPointer<MemoryBlock> new_block) { storage_block = new_block; }
    std::string get_variable_name() const { return variable_name; }
    void set_variable_name(std::string new_variable_name) { variable_name = new_variable_name; }
    
    bool compare_scope(const MemoryReferenceScope &other) const {
        if(storage_block == other.storage_block
            && variable_name == other.variable_name) return true;
        return false;
    }
    
    operator std::string() const {
        if(get_storage_block())
            return Misc::StreamAsString() << variable_name << ":" << std::string(*get_storage_block());
        return Misc::StreamAsString() << variable_name << ":";
    }
};

class MemoryReference {
private:
    Misc::SmartPointer<MemoryBlock> block;
    MemoryReferenceID reference_id;
    Misc::SmartPointer<MemoryReferenceScope> scope;
public:
    MemoryReference();
    virtual ~MemoryReference();
    
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
    void set_block(Misc::SmartPointer<MemoryBlock> new_block) { block = new_block; }
    
    Misc::SmartPointer<MemoryReferenceScope> get_scope() const { return scope; }
    void set_scope(Misc::SmartPointer<MemoryReferenceScope> new_scope) { scope = new_scope; }
    bool compare_scope(const MemoryReferenceScope &other) { return scope->compare_scope(other); }
    
    MemoryReferenceID get_id() const { return reference_id; }
};

} // namespace Platform
} // namespace Aesalon

#endif
