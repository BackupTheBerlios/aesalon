#ifndef AESALON_DISPLAY_DISPLAY_INTIALIZER_H
#define AESALON_DISPLAY_DISPLAY_INTIALIZER_H

#include <string>

#include "misc/Singleton.h"

namespace Aesalon {
namespace Display {

class DisplayInitializer : public Misc::Singleton<DisplayInitializer> {
public:
    DisplayInitializer();
    virtual ~DisplayInitializer();
    
    static std::string available_display_interfaces() {
        std::string interfaces = "";
#ifdef AESALON_BUILD_STDOUT_INTERFACE
        interfaces += "stdout ";
#endif
        
        return interfaces;
    }
};

} // namespace Display
} // namespace Aesalon

#endif
