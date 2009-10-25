#include <iostream>
#include "Initializer.h"
#include "misc/Exception.h"

int main(int argc, char *argv[]) {
    try {
        new Aesalon::Interface::Initializer(argv);
    }
    catch(Aesalon::Misc::Exception e) {
        std::cout << "Run-time exception caught: " << e.get_message() << std::endl;
        return 1;
    }
    delete Aesalon::Interface::Initializer::get_instance();
    return 0;
}
