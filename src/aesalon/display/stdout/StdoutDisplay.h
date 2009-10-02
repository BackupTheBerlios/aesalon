#ifndef AESALON_DISPLAY_STDOUT_DISPLAY_H
#define AESALON_DISPLAY_STDOUT_DISPLAY_H

#include "../BasicDisplay.h"

namespace Aesalon {
namespace Display {
namespace Stdout {

class StdoutDisplay : public BasicDisplay {
public:
    StdoutDisplay();
    virtual ~StdoutDisplay();
    virtual void start();
};

} // namespace Stdout
} // namespace Display
} // namespace Aesalon


#endif
