#include "SymbolObserver.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

SymbolObserver::SymbolObserver() : StringObserver(String::STREAM_OUTPUT) {
}

SymbolObserver::~SymbolObserver() {

}

bool SymbolObserver::notify(Misc::SmartPointer<String> string) {
    if(Initializer::get_instance()->get_controller()->get_state() != GDB_SETUP) return false;
    Misc::SmartPointer<StreamOutput> stream = string.to<StreamOutput>();
    
    
    
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
