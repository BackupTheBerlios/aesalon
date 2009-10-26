#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "NamedPipe.h"
#include "PipeException.h"
#include "PlatformException.h"

namespace Aesalon {
namespace Platform {

NamedPipe::NamedPipe(named_pipe_mode_e type, std::string name) : type(type), pipe_name(name) {
    if(type == NamedPipe::WRITE_PIPE) create_pipe();
    else if(type == NamedPipe::READ_PIPE) open_pipe();
}

NamedPipe::~NamedPipe() {
    if(type == NamedPipe::WRITE_PIPE) remove(pipe_name.c_str());
}

void NamedPipe::send_data(std::string data) {
    if(get_type() != NamedPipe::WRITE_PIPE) throw PipeException("Attempting to write to a non-writable pipe");
    write(pipe_fd, data.data(), data.size());
}

std::string NamedPipe::get_data() {
    if(get_type() == NamedPipe::READ_PIPE) {
        char buffer[1024];
    
        read(pipe_fd, buffer, sizeof(buffer));
        return std::string(buffer);
    }
    throw PipeException("Attempting to read a non-readable pipe");
}

void NamedPipe::create_pipe() {
    if(mkfifo(pipe_name.c_str(), FIFO_PIPE_MODE) != 0) throw PlatformException("Could not create named pipe: ");
    pipe_fd = open(pipe_name.c_str(), O_WRONLY);
    if(pipe_fd == -1) throw PlatformException("Could not open named pipe for writing: ");
}

void NamedPipe::open_pipe() {
    pipe_fd = open(pipe_name.c_str(), O_RDONLY);
    if(pipe_fd == -1) throw PlatformException("Could not open named pipe for reading: ");
}

}  // namespace Platform
}  // namespace Aesalon
