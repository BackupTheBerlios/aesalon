#include "ProgramManager.h"

namespace Aesalon {
namespace Interface {

void ProgramManager::execute() {
    bi_pipe = new Platform::BidirectionalPipe(get_argument_list()->get_argument(0), get_argument_list(), false);
    running = true;
    ptrace_portal = new PTracePortal(bi_pipe->get_child_pid());
    ptrace_portal->attach();
}

} // namespace Interface
} // namespace Aesalon
