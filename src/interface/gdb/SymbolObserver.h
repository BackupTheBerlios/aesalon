#ifndef AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H
#define AESALON_INTERFACE_GDB_SYMBOL_OBSERVER_H

#include "StringObserver.h"
#include "platform/SymbolManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class SymbolObserver : public StringObserver {
private:
    Misc::SmartPointer<Platform::SymbolManager> symbol_manager;
    std::size_t current_symbol;
    Platform::MemoryAddress last_address;
    std::size_t instructions_parsed;
    
    const std::size_t INSTRUCTION_BLOCK_SIZE;
    
    void request_next();
public:
    SymbolObserver(Misc::SmartPointer<StateManager> state_manager,
        Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
        : StringObserver(state_manager), symbol_manager(symbol_manager),
        current_symbol(0), last_address(0), instructions_parsed(0), INSTRUCTION_BLOCK_SIZE(100) {}
    virtual ~SymbolObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
    virtual bool notify(Misc::SmartPointer<ResultRecord> result);
    virtual bool notify(Misc::SmartPointer<StreamOutput> stream);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
