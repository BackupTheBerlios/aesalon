#include "BasicDisplay.h"

namespace Aesalon {
namespace Display {

template<>BasicDisplay *Misc::Singleton<BasicDisplay>::instance = 0;

} // namespace Display
} // namespace Aesalon
