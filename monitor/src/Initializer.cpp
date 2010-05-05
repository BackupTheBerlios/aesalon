/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file Initializer.cpp
    This file contains the implementation of the Initializer class.
*/

#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "misc/StreamAsString.h"
#include "misc/String.h"
#include "misc/Message.h"
#include "event/MonitorEvent.h"

#include "Initializer.h"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT 6321
#endif

#ifndef LIBC_PATH
    #define LIBC_PATH "/lib/libc.so.6"
#endif

template<> Initializer *Misc::Singleton<Initializer>::instance = 0;

Initializer::Initializer(char **argv) : Misc::Singleton<Initializer>(), argv(argv) {
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
    analyzer_interface = NULL;
    return_value = 0;
    scope_manager = NULL;
    
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
    
    argument_parser->add_argument(new Misc::Argument("no-backtrace", 0, Misc::Argument::NO_ARGUMENT, ""));
    std::string chunk_size = config_parser->get_config_item("chunk-size");
    if(chunk_size == "") chunk_size = (Misc::StreamAsString() << CHUNK_SIZE);
    argument_parser->add_argument(new Misc::Argument("chunk-size", 0, Misc::Argument::REQUIRED_ARGUMENT, chunk_size));

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
    
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Analyzing executable . . .");
    analyzer_interface = new Analyzer::Interface();
    analyzer_interface->parse_file(program_manager->get_argument_list()->get_argument(0));
    analyzer_interface->parse_file(argument_parser->get_argument("libc-path")->get_data());
    
    if(argument_parser->get_argument("wait")->is_found()) {
        int number;
        Misc::String::to<int>(
            argument_parser->get_argument("wait")->get_data(), number);
        
        Misc::Message(Misc::Message::DEBUG_MESSAGE, Misc::StreamAsString() << "Waiting for " << number << " TCP connection(s) . . .");
        for(int x = 0; x < number; x ++) {
            server_socket->wait_for_connection();
        }
    }
    
    scope_manager = new Tracker::ScopeManager();
    
    run();
    
    server_socket->disconnect_all();
}

void Initializer::deinitialize() {
    if(scope_manager) delete scope_manager;
    if(analyzer_interface) delete analyzer_interface;
    if(config_parser) delete config_parser;
    if(argument_parser) delete argument_parser;
    if(program_manager) delete program_manager;
    if(server_socket) delete server_socket;
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
    std::cout << "\t--libc-path\t\tThe path to the current version of libc being used. Currently is " << argument_parser->get_argument("libc-path")->get_data() << "." << std::endl;
#ifdef USE_OVERLOAD
    std::cout << "\t--overload-path\t\tThe directory containing the overload libraries. Currently is " << argument_parser->get_argument("overload-path")->get_data() << "." << std::endl;
#endif
    std::cout << "\t--no-backtrace\t\tForces the monitor to only consider the current scope, rather than the full backtrace." << std::endl;
    std::cout << "\t--chunk-size\t\tThe size of the storage chunks. The larger, the more memory used. The smaller, the more processing power used and memory wasted. Currently is " << argument_parser->get_argument("chunk-size")->get_data() << "." << std::endl;
    std::cout << "\t--\t\t\tOptional, denotes the end of the argument list." << std::endl;
}

void Initializer::run() {
    program_manager->execute();
    get_event_queue()->push_event(new Event::MonitorEvent(Event::MonitorEvent::PROGRAM_STARTED));
    while(program_manager->is_running()) {
        program_manager->wait();
        if(event_queue->peek_event()) {
            get_socket()->send_data(event_queue);
        }
        /*server_socket->accept_connections();*/
#ifdef USE_OVERLOAD
        usleep(300);
#endif
    }
#ifdef USE_OVERLOAD
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Processing backlog . . .");
    program_manager->process_backlog();
#endif
    /* Now send off the backlog . . . */
    get_socket()->send_data(event_queue);
}

