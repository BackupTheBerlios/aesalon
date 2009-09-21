#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include "Program.h"
#include "misc/Exception.h"
#include "misc/OutOfMemoryException.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Interface {

void create_listening_thread() {
    pid_t child_pid = fork();
    if(child_pid == 0) {
        
    }
}

void Program::execute() {
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == -1) {
        throw Misc::Exception("Couldn't fork to create another process.");
    }
    program_pipe = new Pipe();
    if(child_pid != 0) {
        create_listening_thread();
        return;
    }
    
    char *ld_preload = getenv("LD_PRELOAD");
    std::string preload_string;
    if(ld_preload) {
        /* NOTE: add the aesalon_malloc shared library to the existing LD_PRELOAD. */
        throw Misc::Exception("Temporary: pre-existing LD_PRELOAD environment variable . . . ");
    }
    else preload_string = get_library_location();
    setenv("LD_PRELOAD", preload_string.c_str(), 1);
    
    std::string pipe_fd = Misc::StreamAsString() << program_pipe->get_write_pipe_fd();
    setenv("AESALON_PIPE", pipe_fd.c_str(), 1);
    
    char **arguments;
    std::size_t arguments_size = 0;
    
    std::string arguments_string = get_filename() + " " + get_arguments();
    
    while(arguments_string.size() > 1) {
        /* First, remove whitespace */
        while(arguments_string[0] == ' ' || arguments_string[0] == '\t'
            || arguments_string[0] == '\n') arguments_string.erase(0, 1);
        /* NOTE: handle quotes in here later */
        std::size_t length = 0;
        while(length < arguments_string.size() && arguments_string[length] != ' ' && arguments_string[length] != '\t') length ++;
        arguments = (char **)realloc(arguments, (arguments_size+2) * sizeof(char *));
        arguments[arguments_size] = (char *)malloc((length+1) * sizeof(char));
        for(std::size_t x = 0; x < length; x ++) {
            arguments[arguments_size][x] = arguments_string[x];
            arguments[arguments_size][x+1] = 0;
        }
        arguments_size ++;
        *arguments[arguments_size] = 0;
    }
    
    // strcpy(arguments[0], strrchr(get_filename().c_str(), '/'));
    if(execv(get_filename().c_str(), arguments) == -1) {
        /* then couldn't find executable file, or file not executable . . . */
        throw Misc::Exception("Couldn't execute file.");
    }
    /* will never get here */
}

} // namespace Interface
} // namespace Aesalon
