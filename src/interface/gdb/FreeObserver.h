#ifndef AESALON_INTERFACE_GDB_FREE_OBSERVER_H
#define AESALON_INTERFACE_GDB_FREE_OBSERVER_H

#include "StringObserver.h"
#include "platform/EventQueue.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class FreeObserver : public StringObserver {
private:
    Misc::SmartPointer<Platform::EventQueue> event_queue;
    Platform::MemoryAddress block_address;
public:
    FreeObserver(Misc::SmartPointer<Platform::EventQueue> event_queue)
        : StringObserver(), event_queue(event_queue) {}
    virtual ~FreeObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
