#ifndef AESALON_DISPLAY_BASIC_DISPLAY_H
#define AESALON_DISPLAY_BASIC_DISPLAY_H

#include "misc/Singleton.h"

namespace Aesalon {
namespace Display {

class BasicDisplay : public Misc::Singleton<BasicDisplay> {
public:
    BasicDisplay() : Misc::Singleton<BasicDisplay>() {}
    virtual ~BasicDisplay() {}
    
    virtual void start() = 0;
};

} // namespace Display
} // namespace Aesalon

#endif
