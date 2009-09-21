#include <unistd.h>

#include "Pipe.h"

namespace Aesalon {
namespace Interface {

Pipe::Pipe() {
    int p_fd[2];
    pipe(p_fd);
    /* Only save the reading end of the pipe. */
    pipe_fd = p_fd[0];
    close(p_fd[1]);
}

Pipe::~Pipe() {
    close(get_pipe_fd());
}

} // namespace Interface
} // namespace Aesalon