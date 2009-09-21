#ifndef AESALON_INTERFACE_PIPE_H
#define AESALON_INTERFACE_PIPE_H

namespace Aesalon {
namespace Interface {

class Pipe {
private:
    int pipe_fd;
public:
    Pipe();
    virtual ~Pipe();
    
    int get_pipe_fd() const { return pipe_fd; }
};

} // namespace Interface
} // namespace Aesalon

#endif
