#include "ProgramManager.h"

namespace Aesalon {
namespace Interface {

void ProgramManager::execute() {
    bi_pipe = new Platform::BidirectionalPipe(get_argument_list()->get_argument(0), get_argument_list(), false);
    
}

} // namespace Interface
} // namespace Aesalon
