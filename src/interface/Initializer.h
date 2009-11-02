#ifndef AESALON_INTERFACE_INITIALIZER_H
#define AESALON_INTERFACE_INITIALIZER_H

#include "misc/SmartPointer.h"
#include "misc/Singleton.h"
#include "platform/NamedPipe.h"
#include "platform/BidirectionalPipe.h"
#include "gdb/Processor.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Interface {

class Initializer : public Misc::Singleton<Initializer> {
private:
    char **argv;
    Misc::SmartPointer<Platform::NamedPipe> named_pipe;
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Interface::GDB::Processor> gdb_processor;
    
    Misc::EventQueue event_queue;
    
    void initialize();
    void deinitialize();
    void send_pid_to_gui();
    
    void usage();
    
    void run();
public:
    Initializer(char **argv);
    virtual ~Initializer();
    
    Misc::SmartPointer<Platform::NamedPipe> get_named_pipe() const { return named_pipe; }
};

} // namespace Interface
} // namespace Aesalon

#endif
