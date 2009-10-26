#ifndef AESALON_PLATFORM_NAMED_PIPE_H
#define AESALON_PLATFORM_NAMED_PIPE_H

#define FIFO_PIPE_MODE 0644

#include <string>

namespace Aesalon {
namespace Platform {

class NamedPipe {
public:
    enum named_pipe_mode_e {
        WRITE_PIPE,
        READ_PIPE,
        PIPE_TYPES
    };
private:
    const named_pipe_mode_e type;
    std::string pipe_name;
    int pipe_fd;
    
    void create_pipe();
    void open_pipe();
public:
    NamedPipe(named_pipe_mode_e type, std::string name);
    virtual ~NamedPipe();
    named_pipe_mode_e get_type() const { return type; }
    
    void send_data(std::string data);
    /* NOTE: blocking function if there is no data waiting. */
    std::string get_data();
};

}  // namespace Platform
}  // namespace Aesalon

#endif
