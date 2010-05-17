#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "SharedMemory.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"
#include "Initializer.h"
#include "exception/PTraceException.h"

namespace PTrace {

SharedMemory::SharedMemory(pid_t pid) {
    
}

SharedMemory::~SharedMemory() {
    munmap(memory, shm_size);
    shm_unlink(filename.c_str());
}

} // namespace PTrace
