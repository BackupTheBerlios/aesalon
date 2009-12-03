#ifndef AESALON_PLATFORM_NAMED_PIPE_H
#define AESALON_PLATFORM_NAMED_PIPE_H

#define FIFO_PIPE_MODE 0644

#include <string>

#include "EventQueue.h"
#include "misc/SmartPointer.h"

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
    bool pipe_open;
    bool create_pipe;
    bool blocking;
    
    void create_named_pipe();
    void open_pipe_read();
    void open_pipe_write();
public:
    NamedPipe(named_pipe_mode_e type, std::string name, bool create_pipe = false, bool blocking = false);
    virtual ~NamedPipe();
    named_pipe_mode_e get_type() const { return type; }
    
    void send_data(std::string data);
    void send_data(Misc::SmartPointer<EventQueue> queue);
    
    void try_open();
    
    /* NOTE: blocking function if there is no data waiting. */
    std::string get_data();
    
    bool is_open() const { return pipe_open; }
};

}  // namespace Platform
}  // namespace Aesalon

#endif
