#ifndef AESALON_INTERFACE_INITIALIZER_H
#define AESALON_INTERFACE_INITIALIZER_H

#include "misc/Singleton.h"

namespace Aesalon {
namespace Interface {

class Initializer : public Misc::Singleton<Initializer> {
private:
    void initialize(char *argv[]);
    void deinitialize();
public:
    Initializer(char *argv[]);
    virtual ~Initializer();
};

} // namespace Interface
} // namespace Aesalon

#endif
