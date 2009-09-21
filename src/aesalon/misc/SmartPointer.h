#ifndef AESALON_SMART_POINTER_H
#define AESALON_SMART_POINTER_H

#include <cstddef>

#include "NullPointerException.h"

namespace Aesalon {
namespace Misc {

template<typename Type>
class SmartPointer {
private:
    Type *data;
    void set_data(Type *new_data) { data = new_data; }
    Type *get_data() { return data; }
    Type *get_nonnull_data() {
        if(data) return data;
        throw NullPointerException("Dereferencing Smart Pointer");
    }
public:
    SmartPointer() { set_data(NULL); }
    SmartPointer(Type *data) { set_data(data); }
    SmartPointer(const SmartPointer &other) {
        /* NOTE: this const_cast is ugly. */
        set_data(const_cast<Type *>(other.get_data()));
    }
    
    Type &operator->() {
        return *get_nonnull_data();
    }
    
    Type &operator*() {
        return *get_nonnull_data();
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
