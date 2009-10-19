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
#include "misc/Exception.h"
#include "misc/OutOfMemoryException.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Interface {

Program::Program() {
}

Program::~Program() {

}

void Program::execute(std::string executable) {
    gdb_pipe = new BidirectionalPipe(executable, argument_list);
    pipe_listener = new PipeListener(gdb_pipe);
}

} // namespace Interface
} // namespace Aesalon
