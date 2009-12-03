#include "Program.h"
#include "MainWindow.h"
#include "misc/String.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

Program::Program(std::string executable, std::string arguments, bool in_xterm)
    : executable(executable), arguments(arguments), in_xterm(in_xterm) {
    
    if(in_xterm) {
        arguments = "-e " + executable + " " + arguments;
        executable = "/usr/bin/x-terminal-emulator";
    }
    
    arguments = Misc::StreamAsString() << "/home/strange/c/aesalon/aesalon" << " --gui-pid " << getpid() << " " << executable << " " << arguments;
    executable = "/home/strange/c/aesalon/aesalon"; /* NOTE: hardcoded . . . */
    
    Platform::ArgumentList argument_list;
    
    argument_list.from_string(arguments + " ");
    
    bi_pipe = new Platform::BidirectionalPipe(executable, argument_list, true);
    
    Misc::SmartPointer<Platform::NamedPipe> gui_pipe
        = new Aesalon::Platform::NamedPipe(Aesalon::Platform::NamedPipe::READ_PIPE, Misc::StreamAsString() << "/tmp/aesalon_gui-" << getpid());
    
    std::string data;
    
    gui_pipe->try_open();
    
    while((data = gui_pipe->get_data()) == "") ;
    
    gui_pipe = 0;
    
    std::size_t aesalon_pid = 0;
    
    Misc::String::to<std::size_t>(data, aesalon_pid);
    
    memory = new Platform::Memory();
    named_pipe = new Platform::NamedPipe(Aesalon::Platform::NamedPipe::READ_PIPE,  Misc::StreamAsString() << "/tmp/aesalon-" << aesalon_pid);
}

} // namespace GUI
} // namespace Aesalon
