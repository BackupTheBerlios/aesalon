#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include "Program.h"
#include "misc/Exception.h"
#include "misc/OutOfMemoryException.h"

namespace Aesalon {
namespace Interface {

void Program::execute() {
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == -1) {
        throw Misc::Exception("Couldn't fork to create another process.");
    }
    if(child_pid != 0) {
        program_pipe = new Pipe();
        return;
    }
    
    /* add LD_PRELOAD with get_library_path() */
    char **arguments;
    
    get_arguments();
    
    
    // strcpy(arguments[0], strrchr(get_filename().c_str(), '/'));
    if(execv(get_filename().c_str(), arguments) == -1) {
        /* then couldn't find executable file, or file not executable . . . */
        throw Misc::Exception("Couldn't execute file.");
    }
    /* will never get here */
}

} // namespace Interface
} // namespace Aesalon
