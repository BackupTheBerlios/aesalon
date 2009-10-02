#include <iostream>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"
#include "display/DisplayInitializer.h"

int main(int argc, char *argv[]) {
    new Aesalon::Misc::ReferenceCounter();
    Aesalon::Misc::ArgumentParser *ap = new Aesalon::Misc::ArgumentParser();
    
    ap->add_argument("display interface", new Aesalon::Misc::StringArgument("--interface", 'I', "stdout"));
    ap->add_argument("usage", new Aesalon::Misc::BooleanArgument("--usage", 'h', "", 0, false));
    
    try {
        Aesalon::Misc::ArgumentParser::get_instance()->parse_argv(argv);
    }
    catch(Aesalon::Misc::UnknownArgumentException e) {
        std::cout << e.get_message() << std::endl;
    }
    new Aesalon::Display::DisplayInitializer();
    
    /* The above call will begin the whole process, so clean up afterwards. */
    
    delete Aesalon::Display::DisplayInitializer::get_instance();
    delete Aesalon::Misc::ArgumentParser::get_instance();
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    return 0;
}
