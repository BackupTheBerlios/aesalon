#include "StdoutDisplay.h"
#include <iostream>

namespace Aesalon {
namespace Display {
namespace Stdout {

StdoutDisplay::StdoutDisplay() : BasicDisplay() {
    
}

StdoutDisplay::~StdoutDisplay() {
    
}

void StdoutDisplay::start() {
    std::cout << "Initializing stdout display interface." << std::endl;
}

} // namespace Stdout
} // namespace Display
} // namespace Aesalon
