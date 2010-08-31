#ifndef Polisher_H
#define Polisher_H

#include "polisher/Polisher.h"

class CpuTimePolisher : public PolisherInterface {
public:
    CpuTimePolisher();
    virtual ~CpuTimePolisher();
private:
public:
    virtual DataPacket *handlePacket(DataPacket *packet);
};

#endif
