#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"
#include "misc/StreamAsString.h"

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
    ap->add_argument("gui pid", new Misc::StringArgument("--gui-pid", 0, ""));
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) {
        usage();
        return;
    }
    
    send_pid_to_gui();
    
    named_pipe = new Platform::NamedPipe(Platform::NamedPipe::WRITE_PIPE, Misc::StreamAsString() << "/tmp/aesalon-" << getpid(), true);
    
    if(ap->get_files()) {
        Platform::ArgumentList al;
        al.add_argument(ap->get_argument("gdb executable").to<Misc::StringArgument>()->get_value());
        al.add_argument("--interpreter=mi2");
        al.add_argument(ap->get_file(0)->get_filename());
        /*for(std::size_t x = 0; x < ap->get_files(); x ++) al.add_argument(ap->get_file(x)->get_filename());*/
        bi_pipe = new Platform::BidirectionalPipe(ap->get_argument("gdb executable").to<Misc::StringArgument>()->get_value(), al);
    }
    else {
        usage();
        return;
    }
    
    event_queue = new Misc::EventQueue();
    
    symbol_manager = new SymbolManager();
    
    symbol_manager->parse_from_executable(ap->get_file(0)->get_filename());
    
    gdb_controller = new GDB::Controller(bi_pipe, event_queue);
    
    run();
}

void Initializer::deinitialize() {
    if(symbol_manager) delete symbol_manager;
    if(named_pipe) delete named_pipe;
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
    std::cout << "Internal arguments (do not use, provided for reference):" << std::endl;
    std::cout << "\t--gui-pid\t\tSets the aesalon gui interface PID." << std::endl;
}

void Initializer::run() {
    while(bi_pipe->is_open()) {
        gdb_controller->listen();
        sleep(1);
    }
}

void Initializer::send_pid_to_gui() {
    std::string pid_string = Misc::ArgumentParser::get_instance()->get_argument("gui pid").to<Misc::StringArgument>()->get_value();
    if(pid_string == "") return;
    
    try {
        Platform::NamedPipe named_pipe(Aesalon::Platform::NamedPipe::WRITE_PIPE, Misc::StreamAsString() << "/tmp/aesalon_gui-" << pid_string);
    
        if(named_pipe.is_open()) named_pipe.send_data(Misc::StreamAsString() << getpid());
    }
    catch(Misc::Exception pe) {
        std::cout << pe.get_message() << std::endl;
    }
    /* named_pipe will auto-destruct, since it is a local variable */
}

} // namespace Interface
} // namespace Aesalon
