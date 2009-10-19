#ifndef AESALON_INTERFACE_BIDIRECTIONAL_PIPE_H
#define AESALON_INTERFACE_BIDIRECTIONAL_PIPE_H

#include <string>

#include "ArgumentList.h"
#include "misc/Exception.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Interface {

class BidirectionalPipeException : public Misc::Exception {
public:
    BidirectionalPipeException(std::string message, bool strerr = false);
};

class BidirectionalPipe {
private:
    int pc_pipe_fd[2];
    int cp_pipe_fd[2];
    std::string executable;
    bool is_open;
public:
    BidirectionalPipe(std::string executable, ArgumentList argument_list);
    virtual ~BidirectionalPipe();
    
    void send_string(std::string data);
    std::string get_string();
    
    bool is_connected() const { return is_open; }
};

} // namespace Interface
} // namespace Aesalon

#endif
