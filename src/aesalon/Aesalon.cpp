#include <iostream>
#include "interface/Program.h"
#include "misc/Exception.h"

int main(int argc, char *argv[]) {
    Aesalon::Interface::Program program;
    program.set_arguments("");
    program.set_filename("/home/strange/c/malloc_test");
    program.set_library_location("/home/strange/c/aesalon/src/overload/libaesalon_overload.so");
    try {
        program.execute();
    }
    catch(Aesalon::Misc::Exception e) {
        std::cout << "Exception caught: " << e.get_message() << std::endl;
    }
    return 0;
}

