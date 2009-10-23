#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <exception>
#include <cstdio>

#include "Program.h"
#include "PipeListener.h"
#include "Memory.h"
#include "misc/Exception.h"
#include "misc/OutOfMemoryException.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

Program::Program() {
    program_memory = new Memory();
    event_queue = new Misc::EventQueue();
}

Program::~Program() {
    if(program_memory.is_valid()) delete program_memory;
    if(event_queue.is_valid()) delete event_queue;
    if(gdb_pipe.is_valid()) delete gdb_pipe;
    if(pipe_listener.is_valid()) delete pipe_listener;
}

void Program::execute(std::string executable) {
    gdb_pipe = new BidirectionalPipe(executable, argument_list);
    pipe_listener = new PipeListener(gdb_pipe);
}

} // namespace Platform
} // namespace Aesalon
