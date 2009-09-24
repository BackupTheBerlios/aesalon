#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "interface/Program.h"
#include "misc/Exception.h"
#include "misc/EventQueue.h"
#include "interface/MemoryEvent.h"

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
    catch(std::exception e) {
        std::cout << "std::exception caught: " << e.what() << std::endl;
    }
    
    wait(NULL);
    
    Aesalon::Misc::Event *e;
    
    while((e = Aesalon::Misc::EventQueue::get_instance()->peek_event())) {
        std::cout << "Event found: " << Aesalon::Misc::EventQueue::get_instance()->peek_event() << std::endl;
        Aesalon::Interface::MemoryEvent *me = reinterpret_cast<Aesalon::Interface::MemoryEvent *>(Aesalon::Misc::EventQueue::get_instance()->peek_event());
        std::cout << "\tevent scope: " << me->get_scope() << std::endl;
        Aesalon::Misc::EventQueue::get_instance()->pop_event();
    }
    
    delete Aesalon::Misc::EventQueue::get_instance();
    
    return 0;
}
