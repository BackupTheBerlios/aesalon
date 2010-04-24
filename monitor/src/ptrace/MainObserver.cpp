#include <iostream>

#include "MainObserver.h"
#include "Initializer.h"

namespace PTrace {

MainObserver::MainObserver() {

}

MainObserver::~MainObserver() {

}

bool MainObserver::handle_breakpoint(PTrace::Breakpoint *breakpoint) {
    Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser()->parse_maps();
    breakpoint->remove_observer(this);
    return false;
}


} // namespace PTrace
