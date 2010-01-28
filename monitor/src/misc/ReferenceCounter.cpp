#include <iostream>
#include "ReferenceCounter.h"

namespace Aesalon {
namespace Misc {

template<> ReferenceCounter *Singleton<ReferenceCounter>::instance = 0;

} // namespace Misc
} // namespace Aesalon
