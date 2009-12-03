#include "Program.h"
#include "MainWindow.h"

namespace Aesalon {
namespace GUI {

Program::Program(std::string executable, std::string arguments, bool in_xterm)
    : executable(executable), arguments(arguments), in_xterm(in_xterm) {
    
    if(in_xterm) {
        arguments = "-e " + executable + " " + arguments;
        executable = "/usr/bin/x-terminal-emulator";
    }
    
    Platform::ArgumentList argument_list;
    
    argument_list.from_string(arguments);
    
    bi_pipe = new Platform::BidirectionalPipe(executable, argument_list, true);
    
    /* NOTE: get the real PID from MainWindow::named_pipe . . . */
    std::size_t aesalon_pid = 1;
    
    memory = new Platform::Memory();
    named_pipe = new Platform::NamedPipe(Aesalon::Platform::NamedPipe::READ_PIPE,  Misc::StreamAsString() << "/tmp/aesalon-" << aesalon_pid);
}



} // namespace GUI
} // namespace Aesalon
