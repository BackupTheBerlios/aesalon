#include <iostream>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"
#include "misc/EventQueue.h"

int main(int argc, char *argv[]) {
    new Aesalon::Misc::ReferenceCounter();
    new Aesalon::Misc::EventQueue();
    Aesalon::Misc::ArgumentParser *ap = new Aesalon::Misc::ArgumentParser();
    
    ap->add_argument("display interface", new Aesalon::Misc::StringArgument("--interface", 'I', "stdout"));
    ap->add_argument("usage", new Aesalon::Misc::BooleanArgument("--usage", 'h', "", 0, false));
    ap->add_argument("aesalon library path", new Aesalon::Misc::StringArgument("--aesalon-library-path", 0, "./libaesalon_overload.so"));
    
    try {
        Aesalon::Misc::ArgumentParser::get_instance()->parse_argv(argv);
    }
    catch(Aesalon::Misc::UnknownArgumentException e) {
        std::cout << e.get_message() << std::endl;
    }
    try {
        /* Begin display execution */
    }
    catch(Aesalon::Misc::Exception e) {
        std::cout << "Exception caught: " << e.get_message() << std::endl;
    }
    
    /* The above call will begin the whole process, so clean up afterwards. */
    
    delete Aesalon::Misc::ArgumentParser::get_instance();
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    return 0;
}
