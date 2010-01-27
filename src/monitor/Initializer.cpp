#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"
#include "Message.h"

#include "platform/PlatformException.h"

#include "Initializer.h"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT 6321
#endif

#ifndef LIBC_PATH
    #define LIBC_PATH "/lib/libc.so.6"
#endif

namespace Aesalon {
namespace Monitor {

template<> Initializer *Misc::Singleton<Initializer>::instance = 0;

Initializer::Initializer(char *argv[]) : Misc::Singleton<Initializer>(), argv(argv) {
    initialize();
}

Initializer::~Initializer() {
    deinitialize();
}

void Initializer::initialize() {
    new Misc::ReferenceCounter();
    Misc::ArgumentParser *ap = new Misc::ArgumentParser();
    
    ap->add_argument("usage", new Misc::BooleanArgument("--usage", 'h', "", 0, false));
    ap->add_argument("logfile", new Misc::StringArgument("--log-file", 'l', ""));
    ap->add_argument("tcp port", new Misc::StringArgument("--use-port", 0, Misc::StreamAsString() << DEFAULT_PORT));
    ap->add_argument("wait", new Misc::BooleanArgument("--wait", 'w', "", 0, false));
    ap->add_argument("wait for", new Misc::StringArgument("--wait-for", 0, "1"));
    ap->add_argument("libc path", new Misc::StringArgument("--libc-path", 0, LIBC_PATH));
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) {
        usage();
        return;
    }
    
    if(ap->get_files()) {
        int port;
        Misc::String::to<int>(ap->get_argument("tcp port").to<Misc::StringArgument>()->get_value(), port);
        server_socket = new Platform::TCPServerSocket(port);
        
        Misc::SmartPointer<Platform::ArgumentList> al = new Platform::ArgumentList;
        /*al->add_argument(ap->get_file(0)->get_filename());*/
        for(std::size_t x = 0; x < ap->get_files(); x ++) {
            al->add_argument(ap->get_file(x)->get_filename());
        }
        program_manager = new ProgramManager(al);
    }
    else {
        usage();
        return;
    }
    
    event_queue = new Platform::EventQueue();
    
    /*symbol_manager = new Platform::SymbolManager();*/
    
    /*symbol_manager->parse_from_executable(ap->get_file(0)->get_filename());*/
    
    if(ap->get_argument("wait").to<Misc::BooleanArgument>()->get_status()) {
        int number;
        Misc::String::to<int>(
            ap->get_argument("wait for").to<Misc::StringArgument>()->get_value(), number);
        
        Message(Message::DEBUG_MESSAGE, Misc::StreamAsString() << "Waiting for " << number << " TCP connection(s) . . .");
        for(int x = 0; x < number; x ++) {
            server_socket->wait_for_connection();
        }
    }
    
    run();
    
    server_socket->disconnect_all();
}

void Initializer::deinitialize() {
    if(program_manager) delete program_manager;
    /*if(symbol_manager) delete symbol_manager;*/
    if(server_socket) delete server_socket;
    if(event_queue) delete event_queue;
    
    Misc::ArgumentParser::lock_mutex();
    delete Misc::ArgumentParser::get_instance();
    Misc::ReferenceCounter::lock_mutex();
    delete Misc::ReferenceCounter::get_instance();
}

void Initializer::usage() {
    std::cout << "aesalon program monitor, version " << AESALON_MAJOR_VERSION << "." << AESALON_MINOR_VERSION << "." << AESALON_PATCHLEVEL;
    std::cout << ", copyright (C) 2009-2010 strange <kawk256@gmail.com>" << std::endl;
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--usage, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--log-file, -l\t\tSet the file to log memory events to, for future reconstruction." << std::endl;
    std::cout << "\t--use-port\t\tSet the port to listen on for connections. Defaults to " << DEFAULT_PORT << "." << std::endl;
    std::cout << "\t--wait, -w\t\tWait for a TCP connection before executing. Defaults to false." << std::endl;
    std::cout << "\t--libc-path\t\tThe path to the current version of libc being used. Defaults to " << LIBC_PATH << "." << std::endl;
}

void Initializer::run() {
    program_manager->execute();
    while(program_manager->is_running()) {
        program_manager->wait();
        if(event_queue->peek_event().is_valid()) {
            std::cout << "Sending data from event queue . . ." << std::endl;
            get_socket()->send_data(event_queue);
        }
        server_socket->accept_connections();
    }
}

} // namespace Monitor
} // namespace Aesalon
