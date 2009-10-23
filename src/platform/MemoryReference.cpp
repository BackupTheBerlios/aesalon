#include "MemoryReference.h"

namespace Aesalon {
namespace Platform {

MemoryReference::MemoryReference() {
    /* NOTE: the following may or may not be thread-safe */
    static MemoryReferenceID last_id_number = 1;
    reference_id = last_id_number ++;
}

MemoryReference::~MemoryReference() {
    
}

} // namespace Platform
} // namespace Aesalon
