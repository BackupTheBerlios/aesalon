#ifndef AESALON_PLATFORM_MEMORY_REFERENCE_H
#define AESALON_PLATFORM_MEMORY_REFERENCE_H

#include <string>

#include "MemoryBlock.h"
#include "misc/SmartPointer.h"

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
};

class MemoryReference : public MemoryReferenceScope {
private:
    Misc::SmartPointer<MemoryBlock> block;
    MemoryReferenceID reference_id;
public:
    MemoryReference();
    virtual ~MemoryReference();
    
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
    void set_block(Misc::SmartPointer<MemoryBlock> new_block) { block = new_block; }
    
    MemoryReferenceID get_id() const { return reference_id; }
};

} // namespace Platform
} // namespace Aesalon

#endif
