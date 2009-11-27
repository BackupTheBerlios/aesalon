#ifndef AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H
#define AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H

#include <string>

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class SymbolObserver : public StringObserver {
private:
    std::string current_scope;
    std::vector<std::string> parsed_scopes;
    bool finished_parsing;
public:
    SymbolObserver();
    virtual ~SymbolObserver();
    
    virtual bool notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
