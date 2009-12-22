#include <QSettings>

#include "Program.h"
#include "MainWindow.h"
#include "misc/String.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

Program::Program() {
    QSettings settings;
    
    arguments = settings.value("Program/arguments").toString().toStdString();
    executable = settings.value("Program/executable").toString().toStdString();
    port = settings.value("Program/port").toInt();
    in_xterm = settings.value("Program/in-xterm").toBool();
    
    arguments = Misc::StreamAsString() << settings.value("Core/aesalon-path").toString().toStdString()
        << " --use-port " << port << " " << executable << " " << arguments;
    
    Platform::ArgumentList argument_list;
    
    argument_list.from_string(arguments + " ");
    
    bi_pipe = new Platform::BidirectionalPipe(settings.value("Core/aesalon-path").toString().toStdString(), argument_list, true, false);
    
    memory = new Platform::Memory();
    
    socket = NULL;
    while(!socket.is_valid()) {
        try {
            socket = new Platform::TCPSocket("127.0.0.1", port);
        }
        catch(Platform::PlatformException pe) {
            socket = NULL;
        }
    }
}

} // namespace GUI
} // namespace Aesalon
