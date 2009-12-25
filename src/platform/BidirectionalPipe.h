#ifndef AESALON_PLATFORM_BIDIRECTIONAL_PIPE_H
#define AESALON_PLATFORM_BIDIRECTIONAL_PIPE_H

#include <string>
#include <sys/types.h>

#include "ArgumentList.h"
#include "misc/Exception.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

class BidirectionalPipeException : public Misc::Exception {
public:
    BidirectionalPipeException(std::string message, bool strerr = false);
};

class BidirectionalPipe {
private:
    int pc_pipe_fd[2];
    int cp_pipe_fd[2];
    std::string executable;
    bool is_connected;
    bool block;
    pid_t pid;
public:
    BidirectionalPipe(std::string executable, ArgumentList argument_list, bool block);
    virtual ~BidirectionalPipe();
    
    void send_string(std::string data);
    std::string get_string();
    
    void set_blocking(bool new_blocking);
    
    pid_t get_child_pid() const { return pid; }
    
    bool is_open() const { return is_connected; }
    bool is_blocking() const { return block; }
};

} // namespace Platform
} // namespace Aesalon

#endif
