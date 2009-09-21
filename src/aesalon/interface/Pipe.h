#ifndef AESALON_INTERFACE_PIPE_H
#define AESALON_INTERFACE_PIPE_H

namespace Aesalon {
namespace Interface {

class Pipe {
private:
    int pipe_fd;
    int write_fd;
public:
    Pipe();
    virtual ~Pipe();
    
    int get_pipe_fd() const { return pipe_fd; }
    int get_write_pipe_fd() const { return write_fd; }
};

} // namespace Interface
} // namespace Aesalon

#endif
