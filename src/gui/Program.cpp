#include <QSettings>

#include "Program.h"
#include "MainWindow.h"
#include "misc/String.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

Program::Program() {
    std::cout << "Program::Program(): Beginning initialization . . ." << std::endl;
    QSettings settings;
    
    arguments = settings.value("Program/arguments").toString().toStdString();
    executable = settings.value("Program/executable").toString().toStdString();
    port = settings.value("Program/port").toInt();
    in_xterm = settings.value("Program/in-xterm").toBool();
    
    arguments = Misc::StreamAsString() << settings.value("Core/aesalon-path").toString().toStdString()
        << " --use-port " << port << " " << executable << " " << arguments;
    
    Platform::ArgumentList argument_list;
    
    argument_list.from_string(arguments + " ");
    
    std::cout << "Program::Program(): launching aesalon gdb interface . . ." << std::endl;
    bi_pipe = new Platform::BidirectionalPipe(settings.value("Core/aesalon-path").toString().toStdString(), argument_list, true);
    std::cout << "Program::Program(): launched aesalon gdb interface!" << std::endl;
    
    memory = new Platform::Memory();
    
    std::cout << "Program::Program(): Declared Memory instance, creating socket for communications . . ." << std::endl;
    
    socket = new Platform::TCPSocket("127.0.0.1", port);
    
    std::cout << "Program::Program(): Exiting from constructor . . ." << std::endl;
}

} // namespace GUI
} // namespace Aesalon
