#include <string>
#include "misc/ArgumentParser.h"

#include "Initializer.h"

namespace Aesalon {
namespace Interface {

template<> Initializer *Misc::Singleton<Initializer>::instance = 0;

Initializer::Initializer(char *argv[]) : Misc::Singleton<Initializer>() {
    initialize(argv);
}

Initializer::~Initializer() {
    deinitialize();
}

void Initializer::initialize(char *argv[]) {
    new Aesalon::Misc::ReferenceCounter();
    Aesalon::Misc::ArgumentParser *ap = new Aesalon::Misc::ArgumentParser();
    
    ap->add_argument("usage", new Aesalon::Misc::BooleanArgument("--usage", 'h', "", 0, false));
    
    try {
        ap->parse_argv(argv);
    }
    catch(Aesalon::Misc::ArgumentException ae) {
        
    }
    catch(Aesalon::Misc::Exception e) {
    
    }
}

void Initializer::deinitialize() {
    Aesalon::Misc::ArgumentParser::lock_mutex();
    delete Aesalon::Misc::ArgumentParser::get_instance();
    Aesalon::Misc::ReferenceCounter::lock_mutex();
    delete Aesalon::Misc::ReferenceCounter::get_instance();
}

} // namespace Interface
} // namespace Aesalon