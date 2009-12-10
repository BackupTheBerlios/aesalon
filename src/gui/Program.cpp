#include "Program.h"
#include "MainWindow.h"
#include "misc/String.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

Program::Program(std::string executable, std::string arguments, int port, bool in_xterm)
    : executable(executable), arguments(arguments), port(port), in_xterm(in_xterm) {
    
    if(in_xterm) {
        arguments = "-e " + executable + " " + arguments;
        executable = "/usr/bin/x-terminal-emulator";
    }
    
    arguments = Misc::StreamAsString() << "/home/strange/c/aesalon/aesalon" << " --use-port " << port << " " << executable << " " << arguments;
    executable = "/home/strange/c/aesalon/aesalon"; /* NOTE: hardcoded . . . */
    
    Platform::ArgumentList argument_list;
    
    argument_list.from_string(arguments + " ");
    
    bi_pipe = new Platform::BidirectionalPipe(executable, argument_list, true);
    
    memory = new Platform::Memory();
    
    socket = new Platform::TCPSocket("127.0.0.1", port);
}

} // namespace GUI
} // namespace Aesalon
