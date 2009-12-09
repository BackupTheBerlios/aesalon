#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"

#include "platform/PlatformException.h"

#include "Initializer.h"

#ifndef AESALON_MAJOR_VERSION
    #define AESALON_MAJOR_VERSION "unknown"
#endif

#ifndef AESALON_MINOR_VERSION
    #define AESALON_MINOR_VERSION "unknown"
#endif

#ifndef AESALON_PATCHLEVEL
    #define AESALON_PATCHLEVEL "unknown"
#endif

#define DEFAULT_PORT 6321

namespace Aesalon {
namespace Interface {

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
    ap->add_argument("gdb executable", new Misc::StringArgument("--gdb-path", 0, "/usr/bin/gdb"));
    ap->add_argument("tcp port", new Misc::StringArgument("--use-port", 0, Misc::StreamAsString() << DEFAULT_PORT));
    ap->add_argument("overload path", new Misc::StringArgument("--overload-path", 0, "./libaesalon_overload.so"));
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) {
        usage();
        return;
    }
    
    int port;
    Misc::String::to<int>(ap->get_argument("tcp port").to<Misc::StringArgument>()->get_value(), port);
    server_socket = new Platform::TCPServerSocket(port);
    
    if(ap->get_files()) {
        Platform::ArgumentList al;
        al.add_argument(ap->get_argument("gdb executable").to<Misc::StringArgument>()->get_value());
        al.add_argument("--interpreter=mi2");
        al.add_argument(ap->get_file(0)->get_filename());
        /*for(std::size_t x = 0; x < ap->get_files(); x ++) al.add_argument(ap->get_file(x)->get_filename());*/
        bi_pipe = new Platform::BidirectionalPipe(ap->get_argument("gdb executable").to<Misc::StringArgument>()->get_value(), al, true);
    }
    else {
        usage();
        return;
    }
    
    event_queue = new Platform::EventQueue();
    
    symbol_manager = new Platform::SymbolManager();
    
    symbol_manager->parse_from_executable(ap->get_file(0)->get_filename());
    
    gdb_controller = new GDB::Controller(bi_pipe, event_queue, symbol_manager);
    
    run();
}

void Initializer::deinitialize() {
    if(bi_pipe) delete bi_pipe;
    if(symbol_manager) delete symbol_manager;
    if(server_socket) delete server_socket;
    if(gdb_controller) delete gdb_controller;
    if(event_queue) delete event_queue;
    
    Misc::ArgumentParser::lock_mutex();
    delete Misc::ArgumentParser::get_instance();
    Misc::ReferenceCounter::lock_mutex();
    delete Misc::ReferenceCounter::get_instance();
}

void Initializer::usage() {
    std::cout << "aesalon gdb interface, version " << AESALON_MAJOR_VERSION << "." << AESALON_MINOR_VERSION << "." << AESALON_PATCHLEVEL;
    std::cout << ", copyright (C) 2009" << std::endl;
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--usage, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--log-file, -l\t\tSets the file to log memory events to, for future reconstruction." << std::endl;
    std::cout << "\t--gdb-path\t\tSets the path to the gdb executable to use." << std::endl;
    std::cout << "\t--use-port\t\tSets the port to listen on for connections." << std::endl;
}

void Initializer::run() {
    while(bi_pipe->is_open() && gdb_controller->is_running()) {
        gdb_controller->listen();
        if(event_queue->peek_event().is_valid()) get_socket()->send_data(event_queue);
    }
}

} // namespace Interface
} // namespace Aesalon
