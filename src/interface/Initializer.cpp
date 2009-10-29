#include <string>
#include <iostream>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"

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
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) {
        usage();
        return;
    }
    
    named_pipe = new Platform::NamedPipe(Platform::NamedPipe::WRITE_PIPE, Misc::StreamAsString() << "/tmp/aesalon-" << getpid());
    
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
    
    gdb_parser = new GDBParser(bi_pipe);
    
    run();
}

void Initializer::deinitialize() {
    if(named_pipe) delete named_pipe;
    if(gdb_parser) delete gdb_parser;
    
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
}

void Initializer::run() {
    std::string data;
    
    while(bi_pipe->is_open()) {
        gdb_parser->parse(std::cout);
    }
}

} // namespace Interface
} // namespace Aesalon