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
    pipe_listener = new PipeListener(program_pipe, this);
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
        close(program_pipe->get_write_pipe_fd());
        create_listening_thread();
        return;
    }
    close(program_pipe->get_pipe_fd());
    char *ld_preload = getenv("LD_PRELOAD");
    std::string preload_string;
    if(ld_preload) {
        preload_string = Misc::StreamAsString() << ld_preload << " " << get_library_location();
    }
    else preload_string = get_library_location();
    setenv("LD_PRELOAD", preload_string.c_str(), 1);
    
    std::string pipe_fd = Misc::StreamAsString() << program_pipe->get_write_pipe_fd();
    setenv("AESALON_OVERLOAD_PIPE", pipe_fd.c_str(), 1);
    
    char **arguments = NULL;
    std::size_t arguments_size = 0;
    
    std::string arguments_string = get_filename() + " " + get_arguments();
    while(arguments_string.size()) {
        arguments = (char **)realloc(arguments, sizeof(char *) * (arguments_size + 2));
        std::string argument = arguments_string.substr(0, arguments_string.find(' '));
        arguments[arguments_size] = (char *)malloc(sizeof(char) * argument.size()+1);
        strcpy(arguments[arguments_size], argument.c_str());
        
        arguments_size ++;
        arguments[arguments_size] = 0;
        arguments_string.erase(0, arguments_string.find(' ')+1);
    }
    
    // strcpy(arguments[0], strrchr(get_filename().c_str(), '/'));
    if(execvp(get_filename().c_str(), arguments) == -1) {
        /* then couldn't find executable file, or file not executable . . . */
        throw Misc::Exception(Misc::StreamAsString() << "Couldn't execute file: " << strerror(errno));
    }
    /* will never get here */
}

std::string Program::resolve_address(std::size_t address) {
    return program_parser->find_name_by_address(address);
}

Program::~Program() {
    if(program_pipe) delete program_pipe;
    if(pipe_listener) delete pipe_listener;
    if(program_parser) delete program_parser;
}

} // namespace Interface
} // namespace Aesalon
