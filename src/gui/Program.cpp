#include <QSettings>
#include <unistd.h>

#include "Program.h"
#include "MainWindow.h"
#include "misc/String.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

ProgramSocketThread::ProgramSocketThread(Misc::SmartPointer<Platform::TCPSocket> socket,
    Misc::SmartPointer<Platform::Memory> memory) : socket(socket), memory(memory) {
}

void ProgramSocketThread::run() {
    while(socket->is_valid()) {
        std::cout << "ProgramSocketThread: checking data . . ." << std::endl;
        std::cout << "Received \"" << socket->get_data() << "\"" << std::endl;
        this->msleep(500);
    }
    this->exit(0);
}

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
    
    /*bi_pipe = new Platform::BidirectionalPipe(argument_list, true);*/
    pid_t pid = fork();
    if(pid == 0) {
        std::cout << "in child, execv'ing . . ." << std::endl;
        std::cout << "\targument_list[0]: \"" << argument_list.get_argument(0) << "\"\n";
        std::cout << "\targument_list[1]: \"" << argument_list.get_argument(1) << "\"\n";
        std::cout << "\targument_list[2]: \"" << argument_list.get_argument(2) << "\"\n";
        std::cout << "\targument_list[3]: \"" << argument_list.get_argument(3) << "\"\n";
        std::cout << "\targument_list[4]: \"" << argument_list.get_argument(4) << "\"\n";
        execv(argument_list.get_argument(0).c_str(), argument_list.get_as_argv());
        throw Misc::Exception("Couldn't execute file . . .");
    }
    
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
    
    socket_thread = new ProgramSocketThread(socket, memory);
    socket_thread->start(QThread::HighPriority);
}

} // namespace GUI
} // namespace Aesalon
