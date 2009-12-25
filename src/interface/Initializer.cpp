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
    ap->add_argument("tcp port", new Misc::StringArgument("--use-port", 0, Misc::StreamAsString() << DEFAULT_PORT));
    
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
        al->add_argument(ap->get_file(0)->get_filename());
        program_manager = new ProgramManager(al);
    }
    else {
        usage();
        return;
    }
    
    event_queue = new Platform::EventQueue();
    
    /*symbol_manager = new Platform::SymbolManager();*/
    
    /*symbol_manager->parse_from_executable(ap->get_file(0)->get_filename());*/
    
    run();
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
    std::cout << ", copyright (C) 2009 strange <kawk256@gmail.com>" << std::endl;
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--usage, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--log-file, -l\t\tSets the file to log memory events to, for future reconstruction." << std::endl;
    std::cout << "\t--use-port\t\tSets the port to listen on for connections." << std::endl;
}

void Initializer::run() {
    program_manager->execute();
    while(program_manager->is_running()) {
        program_manager->wait();
        if(event_queue->peek_event().is_valid()) get_socket()->send_data(event_queue);
    }
}

} // namespace Interface
} // namespace Aesalon
