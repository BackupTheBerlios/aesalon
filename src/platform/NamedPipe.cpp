#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "NamedPipe.h"
#include "PipeException.h"
#include "PlatformException.h"

namespace Aesalon {
namespace Platform {

NamedPipe::NamedPipe(named_pipe_mode_e type, std::string name, bool create_pipe) : type(type), pipe_name(name), create_pipe(create_pipe) {
    pipe_open = false;
    if(type == NamedPipe::WRITE_PIPE) open_pipe_write();
    else if(type == NamedPipe::READ_PIPE) open_pipe_read();
}

NamedPipe::~NamedPipe() {
    if(type == NamedPipe::WRITE_PIPE) remove(pipe_name.c_str());
}

void NamedPipe::send_data(std::string data) {
    if(get_type() != NamedPipe::WRITE_PIPE) throw PipeException("Attempting to write to a non-writable pipe");
    else if(!is_open()) {
        /*throw PipeException("Attempting to write to a named pipe that is not open");*/
        /* Try to open the pipe for writing, but just ignore if it fails */
        open_pipe_write();
        if(!is_open()) {
            return;
        }
    }
    int bytes_written = write(pipe_fd, data.data(), data.size());
    if(bytes_written == -1) {
        pipe_open = false;
    }
}

std::string NamedPipe::get_data() {
    if(get_type() != NamedPipe::READ_PIPE) {
        throw PipeException("Attempting to read a non-readable pipe");
    }
    else if(!is_open()) throw PipeException("Attempting to read from a named pipe that is not open");
    
    char buffer[1024] = {0}; /* NOTE: this is slightly inefficient on memory usage . . . */
    int bytes_read = read(pipe_fd, buffer, sizeof(buffer));
    if(bytes_read == -1) {
        pipe_open = false;
    }
    return std::string(buffer);
}

void NamedPipe::create_named_pipe() {
    static bool pipe_created = false;
    if(!pipe_created) {
        if(mkfifo(pipe_name.c_str(), FIFO_PIPE_MODE) != 0) throw PlatformException("Could not create named pipe: ");
        pipe_created = true;
    }
}

void NamedPipe::open_pipe_write() {
    if(create_pipe) create_named_pipe();
    pipe_fd = open(pipe_name.c_str(), O_WRONLY | O_NONBLOCK);
    if(pipe_fd == -1) {
        if(errno != ENXIO) throw PlatformException("Could not open named pipe for writing: ");
        pipe_open = false;
    }
    else pipe_open = true;
}

void NamedPipe::open_pipe_read() {
    if(create_pipe) create_named_pipe();
    pipe_fd = open(pipe_name.c_str(), O_RDONLY | O_NONBLOCK);
    if(pipe_fd == -1) {
        throw PlatformException("Could not open named pipe for reading: ");
    }
    else pipe_open = true;
}

}  // namespace Platform
}  // namespace Aesalon
