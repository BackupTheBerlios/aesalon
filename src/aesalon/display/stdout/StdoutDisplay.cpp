#include "StdoutDisplay.h"
#include "interface/Program.h"
#include "misc/ArgumentParser.h"
#include "misc/Exception.h"
#include "misc/EventQueue.h"
#include "interface/MemoryEvent.h"

namespace Aesalon {
namespace Display {
namespace Stdout {

StdoutDisplay::StdoutDisplay() : BasicDisplay() {
    
}

StdoutDisplay::~StdoutDisplay() {
    
}

void StdoutDisplay::start() {
    Misc::ArgumentParser *ap = Misc::ArgumentParser::get_instance();
    
    if(!ap->get_files()) {
        throw Misc::Exception("No filenames in argument parser");
    }
    Interface::Program program;
    program.set_library_location(ap->get_argument("aesalon library path").to<Misc::StringArgument>()->get_value());
    program.set_filename(ap->get_file(0)->get_filename());
    if(ap->get_files() > 1) {
        std::string program_arguments = "";
        for(std::size_t x = 1; x < ap->get_files(); x ++) {
            program_arguments += ap->get_file(x)->get_filename();
        }
        program.set_arguments(program_arguments);
    }
    else program.set_arguments("");
    
    program.execute();
    
    sleep(1);
    
    Misc::EventQueue *eq = Misc::EventQueue::get_instance();
    
    do {
        Misc::EventQueue::lock_mutex();
        while(eq->peek_event()) {
            if(eq->peek_event()->get_type() == Misc::Event::MEMORY_EVENT) {
                Interface::MemoryEvent *me = dynamic_cast<Interface::MemoryEvent *>(eq->peek_event());
                switch(me->get_memory_type()) {
                    case Interface::MemoryEvent::MALLOC_EVENT: {
                        Interface::MallocEvent *ma = dynamic_cast<Interface::MallocEvent *>(me);
                        if(!me) *((int *)NULL) = 2;
                        std::cerr << "malloc in scope \"" << me->get_scope() << "\", size " << ma->get_size() << std::endl;
                    }
                    default: break;
                }
            }
            eq->pop_event();
        }
        Misc::EventQueue::unlock_mutex();
    } while(program.is_running());
}

} // namespace Stdout
} // namespace Display
} // namespace Aesalon
