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
    new Aesalon::Misc::ReferenceCounter();
    Aesalon::Misc::ArgumentParser *ap = new Aesalon::Misc::ArgumentParser();
    
    ap->add_argument("usage", new Aesalon::Misc::BooleanArgument("--usage", 'h', "", 0, false));
    ap->add_argument("gui pid", new Aesalon::Misc::StringArgument("--gui-pid", 'p', ""));
    
    ap->parse_argv(argv);
    
    if(ap->get_argument("usage").to<Misc::BooleanArgument>()->get_status()) usage();
    std::cout << "GUI pid: " << ap->get_argument("gui pid").to<Aesalon::Misc::StringArgument>()->get_value() << std::endl;
}

void Initializer::deinitialize() {
    Aesalon::Misc::ArgumentParser::lock_mutex();
    delete Aesalon::Misc::ArgumentParser::get_instance();
    Aesalon::Misc::ReferenceCounter::lock_mutex();
    delete Aesalon::Misc::ReferenceCounter::get_instance();
}

void Initializer::usage() {
    std::cout << argv[0] << ": aesalon gdb interface, version v" << MAJOR_VERSION << "." << MINOR_VERSION << "." << PATCHLEVEL;
    std::cout << ", copyright (C) 2009" << std::endl;
}

} // namespace Interface
} // namespace Aesalon