#include <iostream>
#include "interface/Program.h"
#include "misc/Exception.h"
#include "misc/EventQueue.h"

int main(int argc, char *argv[]) {
    Aesalon::Interface::Program program;
    program.set_arguments("");
    program.set_filename("/home/strange/c/malloc_test");
    program.set_library_location("/home/strange/c/aesalon/src/overload/libaesalon_overload.so");
    new Aesalon::Misc::EventQueue();
    try {
        program.execute();
    }
    catch(Aesalon::Misc::Exception e) {
        std::cout << "Exception caught: " << e.get_message() << std::endl;
    }
    
    sleep(5);
    
    std::cout << "main(): EventQueue address is: " << Aesalon::Misc::EventQueue::get_instance() << std::endl;
    
    Aesalon::Misc::Event *e;
    
    while((e = Aesalon::Misc::EventQueue::get_instance()->peek_event())) {
        Aesalon::Misc::EventQueue::get_instance()->pop_event();
    }
    
    delete Aesalon::Misc::EventQueue::get_instance();
    
    return 0;
}
