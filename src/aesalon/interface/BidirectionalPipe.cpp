#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "BidirectionalPipe.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Interface {

BidirectionalPipeException::BidirectionalPipeException(std::string message,
    bool strerr) : Misc::Exception(Misc::StreamAsString() << message << (strerr?strerror(errno):"")) {
}

BidirectionalPipe::BidirectionalPipe(std::string executable,
    ArgumentList argument_list) {
    
    
    pipe(pc_pipe_fd);
    pipe(cp_pipe_fd);
    
    pid_t pid = fork();
    
    if(pid == -1) throw Misc::Exception("Could not fork to execute child process");
    if(pid == 0) {
        close(pc_pipe_fd[1]); /* Close the write end of the pc pipe */
        close(cp_pipe_fd[0]); /* Close the read end of the cp pipe */
        
        fcntl(pc_pipe_fd[0], F_SETFL, fcntl(cp_pipe_fd[0], F_GETFL) & ~O_NONBLOCK);
        fcntl(cp_pipe_fd[1], F_SETFL, fcntl(cp_pipe_fd[1], F_GETFL) & ~O_NONBLOCK);
        
        dup2(pc_pipe_fd[0], STDIN_FILENO);
        dup2(cp_pipe_fd[1], STDOUT_FILENO);
        
        execv(executable.c_str(), argument_list.get_as_argv());
        
        throw BidirectionalPipeException("Failed to execute file: ", true);
    }
    close(pc_pipe_fd[0]);  /* Close the read end of the pc pipe */
    close(cp_pipe_fd[1]);  /* Close the write end of the cp pipe */
    
    fcntl(pc_pipe_fd[1], F_SETFL, fcntl(cp_pipe_fd[1], F_GETFL) & ~O_NONBLOCK);
    fcntl(cp_pipe_fd[0], F_SETFL, fcntl(cp_pipe_fd[0], F_GETFL) & ~O_NONBLOCK);
    
    /* Pipe opened and ready to go. */
    is_open = true;
}

BidirectionalPipe::~BidirectionalPipe() {
    if(is_open) close(pc_pipe_fd[1]), close(cp_pipe_fd[0]);
    is_open = false;
}

std::string BidirectionalPipe::get_string() {
    std::string data;
    char recv;
    
    while((read(cp_pipe_fd[0], &recv, sizeof(recv))) > 0) {
        data += recv;
        if(recv == '\n') return data;
    }
    is_open = false;
    return data;
}

void BidirectionalPipe::send_string(std::string data) {
    if(is_open) write(pc_pipe_fd[1], data.c_str(), data.length());
}


} // namespace Interface
} // namespace Aesalon
