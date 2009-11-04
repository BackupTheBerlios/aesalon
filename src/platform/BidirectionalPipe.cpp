#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include "BidirectionalPipe.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Platform {

BidirectionalPipeException::BidirectionalPipeException(std::string message,
    bool strerr) : Misc::Exception(Misc::StreamAsString() << message << (strerr?strerror(errno):"")) {
}

BidirectionalPipe::BidirectionalPipe(std::string executable,
    ArgumentList argument_list) {
        
    pipe(pc_pipe_fd);
    pipe(cp_pipe_fd);
    
    pid_t pid = fork();
    
    if(pid == -1) throw BidirectionalPipeException("Could not fork to execute child process: ", true);
    else if(pid == 0) {
        close(pc_pipe_fd[1]); /* Close the write end of the pc pipe */
        close(cp_pipe_fd[0]); /* Close the read end of the cp pipe */
        
        fcntl(pc_pipe_fd[0], F_SETFL, fcntl(pc_pipe_fd[0], F_GETFL) & ~O_NONBLOCK);
        fcntl(cp_pipe_fd[1], F_SETFL, fcntl(cp_pipe_fd[1], F_GETFL) & ~O_NONBLOCK);
        
        dup2(pc_pipe_fd[0], STDIN_FILENO);
        dup2(cp_pipe_fd[1], STDOUT_FILENO);
        
        execv(executable.c_str(), argument_list.get_as_argv());
        
        throw BidirectionalPipeException("Failed to execute file: ", true);
    }
    close(pc_pipe_fd[0]);  /* Close the read end of the pc pipe */
    close(cp_pipe_fd[1]);  /* Close the write end of the cp pipe */
    
    fcntl(pc_pipe_fd[1], F_SETFL, fcntl(cp_pipe_fd[1], F_GETFL) & ~O_NONBLOCK);
    /* NOTE: cp_pipe on the parent end needs to be non-blocking! */
    fcntl(cp_pipe_fd[0], F_SETFL, fcntl(cp_pipe_fd[0], F_GETFL) | O_NONBLOCK);
    
    /* Pipe opened and ready to go. */
    is_connected = true;
}

BidirectionalPipe::~BidirectionalPipe() {
    if(is_connected) close(pc_pipe_fd[1]), close(cp_pipe_fd[0]);
    is_connected = false;
}

std::string BidirectionalPipe::get_string() {
    std::string data;
    char recv;
    int ret_value;
    
    while((ret_value = read(cp_pipe_fd[0], &recv, sizeof(recv))) > 0) {
        if(recv == '\n') {
            return data;
        }
        data += recv;
    }
    /* EAGAIN on a pipe read with O_NONBLOCK set means no data waiting. */
    if(ret_value == -1 && errno == EAGAIN) {
        return "";
    }
    is_connected = false;
    return data;
}

void BidirectionalPipe::send_string(std::string data) {
    if(is_open()) write(pc_pipe_fd[1], data.c_str(), data.length());
}


} // namespace Platform
} // namespace Aesalon
