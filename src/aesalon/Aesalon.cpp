#include <iostream>
#include "misc/ArgumentParser.h"
#include "misc/ReferenceCounter.h"

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
    
    std::cout << "Filenames detected: " << Aesalon::Misc::ArgumentParser::get_instance()->get_files() << std::endl;
    std::cout << "Display interface: " << ap->get_argument("display interface").to<Aesalon::Misc::StringArgument>()->get_value() << std::endl;
    std::cout << "Usage: " << ap->get_argument("usage").to<Aesalon::Misc::BooleanArgument>()->get_status() << std::endl;
    
    delete Aesalon::Misc::ArgumentParser::get_instance();
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    return 0;
}
