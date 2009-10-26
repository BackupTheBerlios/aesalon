#ifndef AESALON_INTERFACE_INITIALIZER_H
#define AESALON_INTERFACE_INITIALIZER_H

#include "misc/SmartPointer.h"
#include "misc/Singleton.h"
#include "platform/NamedPipe.h"

namespace Aesalon {
namespace Interface {

class Initializer : public Misc::Singleton<Initializer> {
private:
    char **argv;
    Misc::SmartPointer<Platform::NamedPipe> named_pipe;
    
    void initialize();
    void deinitialize();
    
    void usage();
public:
    Initializer(char **argv);
    virtual ~Initializer();
};

} // namespace Interface
} // namespace Aesalon

#endif
