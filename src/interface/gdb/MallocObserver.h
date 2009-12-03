#ifndef AESALON_INTERFACE_GDB_MALLOC_OBSERVER_H
#define AESALON_INTERFACE_GDB_MALLOC_OBSERVER_H

#include "StringObserver.h"
#include "platform/EventQueue.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class MallocObserver : public StringObserver {
private:
    Misc::SmartPointer<Platform::EventQueue> event_queue;
    std::size_t last_size;
    bool waiting;
public:
    MallocObserver(Misc::SmartPointer<Platform::EventQueue> event_queue)
        : StringObserver(), event_queue(event_queue), waiting(false) {}
    virtual ~MallocObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
