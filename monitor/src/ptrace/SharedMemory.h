#ifndef AESALON_PTRACE_SHARED_MEMORY_H
#define AESALON_PTRACE_SHARED_MEMORY_H

#include <string>

#include "Types.h"

namespace PTrace {

class SharedMemory {
private:
    Byte *memory;
    int shm_fd;
    Word shm_size;
    std::string filename;
public:
    SharedMemory(pid_t pid);
    virtual ~SharedMemory();
    
    Byte *get_memory() const { return memory; }
};

} // namespace PTrace

#endif
