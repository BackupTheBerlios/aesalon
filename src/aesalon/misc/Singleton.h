#ifndef AESALON_MISC_SINGLETON_H
#define AESALON_MISC_SINGLETON_H

#include "Exception.h"

namespace Aesalon {
namespace Misc {

class MultipleSingletonException : public Exception {
public:
    MultipleSingletonException() : Exception("Singleton already initialized") {}
};

class UnitializedSingletonException : public Exception {
public:
    UnitializedSingletonException() : Exception("Singleton not initialized") {}
};

template<typename Type>
class Singleton {
private:
    static Type *instance;
public:
    Singleton() {
        if(instance) throw MultipleSingletonException();
        instance = static_cast<Type *>(this);
    }
    virtual ~Singleton() {
        if(instance) instance = 0;
    }
    
    static Type *get_instance() {
        if(!instance) throw UnitializedSingletonException();
        return instance;
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
