#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <exception>

#include "Program.h"
#include "PipeListener.h"
#include "misc/Exception.h"
#include "misc/OutOfMemoryException.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Interface {

void Program::create_listening_thread() {
    PipeListener pl(program_pipe, this);
}

void Program::execute() {
    pid_t child_pid;
    program_pipe = new Pipe();
    program_parser = new ProgramSymbolParser(get_filename());
    program_parser->parse();
    child_pid = fork();
    if(child_pid == -1) {
        throw Misc::Exception("Couldn't fork to create another process.");
    }
    if(child_pid != 0) {
        create_listening_thread();
        return;
    }
    char *ld_preload = getenv("LD_PRELOAD");
    std::string preload_string;
    if(ld_preload) {
        preload_string = Misc::StreamAsString() << ld_preload << " " << get_library_location();
    }
    else preload_string = get_library_location();
    setenv("LD_PRELOAD", preload_string.c_str(), 1);
    
    std::string pipe_fd = Misc::StreamAsString() << program_pipe->get_write_pipe_fd();
    setenv("AESALON_OVERLOAD_PIPE", pipe_fd.c_str(), 1);
    
    char **arguments;
    std::size_t arguments_size = 0;
    
    std::string arguments_string = get_filename() + " " + get_arguments();
#if 0
    while(arguments_string.size() > 1) {
        std::cout << "arguments_string.size(): " << arguments_string.size() << std::endl;
        /* First, remove whitespace */
        while(arguments_string[0] == ' ' || arguments_string[0] == '\t'
            || arguments_string[0] == '\n') arguments_string.erase(0, 1);
        /* NOTE: handle quotes in here later */
        std::size_t length = 0;
        while(length < arguments_string.size() && arguments_string[length] != ' ' && arguments_string[length] != '\t') length ++;
        std::cout << "Allocating space for another argument . . ." << std::endl;
        arguments = (char **)realloc(arguments, (arguments_size+2) * sizeof(char *));
        std::cout << "Called realloc(), calling malloc() . . ." << std::endl;
        arguments[arguments_size] = (char *)malloc((length+1) * sizeof(char));
        std::cout << "Copying argument from string . . ." << std::endl;
        for(std::size_t x = 0; x < length; x ++) {
            arguments[arguments_size][x] = arguments_string[x];
            arguments[arguments_size][x+1] = 0;
        }
        arguments_size ++;
        *arguments[arguments_size] = 0;
        std::cout << "Checking arguments_string.size() . . ." << std::endl;
    }
#endif
    arguments = new char*[2];
    *arguments[1] = 0;
    arguments[0] = new char[get_filename().size()+1];
    strcpy(arguments[0], get_filename().c_str());
    std::cout << "Executing file '" << get_filename() << "'" << std::endl;
    
    // strcpy(arguments[0], strrchr(get_filename().c_str(), '/'));
    if(execvp(get_filename().c_str(), NULL) == -1) {
        /* then couldn't find executable file, or file not executable . . . */
        throw Misc::Exception(Misc::StreamAsString() << "Couldn't execute file: " << strerror(errno));
    }
    /* will never get here */
}

std::string Program::resolve_address(std::size_t address) {
    return "N/A";
    return program_parser->find_name_by_address(address);
}

} // namespace Interface
} // namespace Aesalon
