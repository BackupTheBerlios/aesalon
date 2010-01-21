#include <iostream>
#include "ReallocObserver.h"
#include "BreakpointReference.h"
#include "platform/BlockEvent.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool ReallocObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    std::cout << "ReallocObserver::handle_breakpoint(): asked to handle breakpoint ID #" << breakpoint->get_id() << std::endl;
    Misc::SmartPointer<Portal> portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    static Word last_address = 0;
    
    if(breakpoint->get_id() != Initializer::get_instance()->get_program_manager()->get_realloc_breakpoint_id()) {
        std::cout << "* ReallocObserver::handle_breakpoint(): return value from realloc() is 0x"
            << std::hex << portal->get_register(ASM::Register::RAX) << std::endl;
        breakpoint->remove_observer(this);
        Initializer::get_instance()->get_event_queue()->push_event(
            new Platform::BlockEvent(Platform::BlockEvent::REALLOC_EVENT,
            last_address, last_size, portal->get_register(ASM::Register::RAX)));
        return true;
    }
    std::cout << "ReallocObserver::handle_breakpoint(): realloc breakpoint found . . ." << std::endl;
    Word rsp = portal->get_register(ASM::Register::RSP);
    std::cout << "\tRBP is: " << std::hex << rsp << std::endl;
    Word return_address = portal->read_memory(rsp);
    /* NOTE: qword [rsp] is where the return address is stored in libc 2.10.2-5, but don't rely on it! */
    return_address = portal->read_memory(rsp);
    std::cout << "\tReturn address: " << return_address << std::endl;
    portal->place_breakpoint(return_address, this);
    std::cout << "\tMemory block size will be " << portal->get_register(ASM::Register::RSI) << std::endl;
    last_size = portal->get_register(ASM::Register::RSI);
    last_address = portal->get_register(ASM::Register::RDI);
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
