#include <iostream>
#include <unistd.h>
#include <cstdio>

#include "Pipe.h"

namespace Aesalon {
namespace Interface {

Pipe::Pipe() {
    int p_fd[2];
    pipe(p_fd);
    pipe_fd = p_fd[0];
    write_fd = p_fd[1];
}

Pipe::~Pipe() {
    close(get_pipe_fd());
    close(get_write_pipe_fd());
}

} // namespace Interface
} // namespace Aesalon