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
    /* set these to NULL so that the usage() function doesn't cause errors . . . */
    program_manager = NULL;
    server_socket = NULL;
    event_queue = NULL;
    return_value = 0;
    storage_manager = NULL;
    
    config_parser = new Misc::ConfigParser();
    
    argument_parser = new Misc::ArgumentParser(argv);
    
    argument_parser->add_argument(new Misc::Argument("help", 'h', Misc::Argument::NO_ARGUMENT, ""));
    argument_parser->add_argument(new Misc::Argument("version", 'v', Misc::Argument::NO_ARGUMENT, ""));
    std::string port = config_parser->get_config_item("tcp-port");
    if(port == "") port = Misc::StreamAsString() << DEFAULT_PORT;
    argument_parser->add_argument(new Misc::Argument("tcp-port", 'p', Misc::Argument::REQUIRED_ARGUMENT, port));
    std::string wait = config_parser->get_config_item("wait");
    if(wait == "") wait = "1";
    argument_parser->add_argument(new Misc::Argument("wait", 'w', Misc::Argument::OPTIONAL_ARGUMENT, wait));
    std::string libc_path = config_parser->get_config_item("libc-path");
    if(libc_path == "") libc_path = LIBC_PATH;
    argument_parser->add_argument(new Misc::Argument("libc-path", 0, Misc::Argument::REQUIRED_ARGUMENT, libc_path));
    std::string overload_path = config_parser->get_config_item("overload-path");
    argument_parser->add_argument(new Misc::Argument("overload-path", 'o', Misc::Argument::REQUIRED_ARGUMENT, overload_path));

    argument_parser->parse();

    if(argument_parser->get_argument("help")->is_found()) {
        usage();
        return;
    }
    else if(argument_parser->get_argument("version")->is_found()) {
        version();
        return;
    }
    
    if(argument_parser->get_argument("overload-path")->get_data() == "")
        throw Exception::BasicException("No overload-path specified.");
    
    storage_manager = new StorageManager();
    
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
    if(config_parser) delete config_parser;
    if(argument_parser) delete argument_parser;
    if(program_manager) delete program_manager;
    if(server_socket) delete server_socket;
    if(storage_manager) delete storage_manager;
    if(event_queue) delete event_queue;
}

void Initializer::version() {
    std::cout << "aesalon program monitor, version " << AESALON_MAJOR_VERSION << "." << AESALON_MINOR_VERSION << "." << AESALON_PATCHLEVEL;
    std::cout << ", copyright (C) 2009-2010 strange <kawk256@gmail.com>" << std::endl;
}

void Initializer::usage() {
    version();
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--version, -v\t\tPrint version information." << std::endl;
    std::cout << "\t--help, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--tcp-port, -p\t\tSet the port to listen on for connections. Currently is " << argument_parser->get_argument("tcp-port")->get_data() << "." << std::endl;
    std::cout << "\t--wait, -w\t\tNumber of TCP connections to accept before executing. Defaults to 0." << std::endl;
    std::cout << "\t--libc-path\t\tThe path to the current version of libc being used. Currently is " << LIBC_PATH << "." << std::endl;
    std::cout << "\t--overload-path\t\tThe path to the aesalon overload library. Currently is " << argument_parser->get_argument("overload-path")->get_data() << "." << std::endl;
    std::cout << "\t--\t\t\tOptional, denotes the end of the argument list." << std::endl;
}

void Initializer::run() {
    program_manager->execute();
    while(program_manager->is_running()) {
        program_manager->wait();
        if(event_queue->peek_event()) {
            get_socket()->send_data(event_queue);
        }
        /*server_socket->accept_connections();*/
    }
}

