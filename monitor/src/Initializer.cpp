#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "misc/StreamAsString.h"
#include "misc/String.h"
#include "misc/Message.h"

#include "Initializer.h"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT 6321
#endif

#ifndef LIBC_PATH
    #define LIBC_PATH "/lib/libc.so.6"
#endif

template<> Initializer *Misc::Singleton<Initializer>::instance = 0;

Initializer::Initializer(char *argv[]) : Misc::Singleton<Initializer>(), argv(argv) {
    initialize();
}

Initializer::~Initializer() {
    deinitialize();
}

void Initializer::initialize() {
    argument_parser = new Misc::ArgumentParser(argv);
    
    argument_parser->add_argument(new Misc::Argument("usage", 'h', Misc::Argument::NO_ARGUMENT, ""));
    argument_parser->add_argument(new Misc::Argument("tcp-port", 'p', Misc::Argument::REQUIRED_ARGUMENT, "6321"));
    argument_parser->add_argument(new Misc::Argument("wait", 'w', Misc::Argument::OPTIONAL_ARGUMENT, "1"));
    argument_parser->add_argument(new Misc::Argument("libc-path", 0, Misc::Argument::REQUIRED_ARGUMENT, LIBC_PATH));

    argument_parser->parse();

    if(argument_parser->get_argument("usage")->is_found()) {
        usage();
        return;
    }
    
    if(argument_parser->get_postargs()) {
        int port;
        Misc::String::to<int>(argument_parser->get_argument("tcp-port")->get_data(), port);
        server_socket = new TCP::ServerSocket(port);
        
        Misc::ArgumentList *al = new Misc::ArgumentList();
        for(std::size_t x = 0; x < argument_parser->get_postargs(); x ++) {
            al->add_argument(argument_parser->get_postarg(x));
        }
        program_manager = new ProgramManager(al);
    }
    else {
        usage();
        return;
    }
    
    event_queue = new Event::Queue();
    
    if(argument_parser->get_argument("wait")->is_found()) {
        int number;
        Misc::String::to<int>(
            argument_parser->get_argument("wait")->get_data(), number);
        
        Misc::Message(Misc::Message::DEBUG_MESSAGE, Misc::StreamAsString() << "Waiting for " << number << " TCP connection(s) . . .");
        for(int x = 0; x < number; x ++) {
            server_socket->wait_for_connection();
        }
    }
    
    run();
    
    server_socket->disconnect_all();
}

void Initializer::deinitialize() {
    if(program_manager) delete program_manager;
    if(server_socket) delete server_socket;
    if(event_queue) delete event_queue;
}

void Initializer::usage() {
    std::cout << "aesalon program monitor, version " << AESALON_MAJOR_VERSION << "." << AESALON_MINOR_VERSION << "." << AESALON_PATCHLEVEL;
    std::cout << ", copyright (C) 2009-2010 strange <kawk256@gmail.com>" << std::endl;
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--usage, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--tcp-port, -p\t\tSet the port to listen on for connections. Defaults to " << DEFAULT_PORT << "." << std::endl;
    std::cout << "\t--wait, -w\t\tWait for a TCP connection before executing. Defaults to false." << std::endl;
    std::cout << "\t--libc-path\t\tThe path to the current version of libc being used. Defaults to " << LIBC_PATH << "." << std::endl;
}

void Initializer::run() {
    program_manager->execute();
    while(program_manager->is_running()) {
        program_manager->wait();
        if(event_queue->peek_event()) {
            std::cout << "Sending data from event queue . . ." << std::endl;
            get_socket()->send_data(event_queue);
        }
        server_socket->accept_connections();
    }
}



