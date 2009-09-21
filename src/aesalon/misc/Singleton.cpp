#include "Singleton.h"

namespace Aesalon {
namespace Misc {

template<typename Type>
Singleton<Type>::Singleton() {
    if(instance) throw MultipleSingletonException();
    instance = static_cast<Type *>(this);
}

template<typename Type>
Singleton<Type>::~Singleton() {
    if(instance) instance = 0;
}

} // namespace Misc
} // namespace Aesalon
