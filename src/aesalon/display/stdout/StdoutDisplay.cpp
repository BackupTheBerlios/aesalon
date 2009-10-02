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
    
    while(program.is_running()) sleep(1);
    
    Misc::EventQueue *eq = Misc::EventQueue::get_instance();
    Misc::EventQueue::lock_mutex();
    
    typedef std::map<std::string, Misc::SmartPointer<ScopeAllocationInformation> > allocation_scope_status_t;
    allocation_scope_status_t allocations;
    
    while(eq->peek_event()) {
        if(eq->peek_event()->get_type() == Misc::Event::MEMORY_EVENT) {
            Interface::MemoryEvent *me = dynamic_cast<Interface::MemoryEvent *>(eq->peek_event());
            switch(me->get_memory_type()) {
                case Interface::MemoryEvent::MALLOC_EVENT: {
                    Interface::MallocEvent *ma = dynamic_cast<Interface::MallocEvent *>(me);
                    ScopeAllocationInformation *scope = allocations[ma->get_scope()];
                    if(scope != NULL) {
                        scope->inc_number();
                        scope->add_size(ma->get_size());
                    }
                    else {
                        allocations[ma->get_scope()] = new ScopeAllocationInformation(1, ma->get_size());
                    }
                }
                default: break;
            }
        }
        eq->pop_event();
    }
    Misc::EventQueue::unlock_mutex();
    
    std::cout << "---------------- aesalon information ----------------" << std::endl;
    allocation_scope_status_t::iterator i = allocations.begin();
    for(; i != allocations.end(); i ++) {
        std::cout << "\tIn the \"" << (*i).first << "\" scope, \n\t\t" << (*i).second->get_number() << " allocations were made"
            << " for a total of " << (*i).second->get_size() << " byte(s)." << std::endl;
    }
}

} // namespace Stdout
} // namespace Display
} // namespace Aesalon
