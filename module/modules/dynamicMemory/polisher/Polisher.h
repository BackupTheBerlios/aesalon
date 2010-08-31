#ifndef Polisher_H
#define Polisher_H

#include "polisher/Polisher.h"

class DynamicMemoryPolisher : public PolisherInterface {
public:
    DynamicMemoryPolisher();
    virtual ~DynamicMemoryPolisher();
private:
public:
    virtual DataPacket *handlePacket(DataPacket *packet);
};

#endif
