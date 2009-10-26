#include <string>
#include <iostream>
#include "misc/ArgumentParser.h"

#include "Initializer.h"

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
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) usage();
    
    named_pipe = new Platform::NamedPipe(Platform::NamedPipe::WRITE_PIPE, Misc::StreamAsString() << "/tmp/aesalon-" << getpid());
}

void Initializer::deinitialize() {
    delete named_pipe;
    
    Misc::ArgumentParser::lock_mutex();
    delete Misc::ArgumentParser::get_instance();
    Misc::ReferenceCounter::lock_mutex();
    delete Misc::ReferenceCounter::get_instance();
}

void Initializer::usage() {
    std::cout << "aesalon gdb interface, version v" << MAJOR_VERSION << "." << MINOR_VERSION << "." << PATCHLEVEL;
    std::cout << ", copyright (C) 2009" << std::endl;
    std::cout << "usage: " << argv[0] << " [arguments] executable [executable arguments]" << std::endl;
    std::cout << "\t--usage, -h\t\tPrint this usage message." << std::endl;
    std::cout << "\t--log-file, -l\t\tSets the file to log memory events to, for future reconstruction." << std::endl;
}

} // namespace Interface
} // namespace Aesalon
