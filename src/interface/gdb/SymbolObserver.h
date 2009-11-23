#ifndef AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H
#define AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class SymbolObserver : public StringObserver {
public:
    SymbolObserver();
    virtual ~SymbolObserver();
    
    virtual bool notify(Misc::SmartPointer<String> string);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
