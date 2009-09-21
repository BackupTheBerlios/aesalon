#ifndef AESALON_SMART_POINTER_H
#define AESALON_SMART_POINTER_H

#include <cstddef>

namespace Aesalon {
namespace Misc {

template<typename Type>
class SmartPointer {
private:
    Type *data;
    void set_data(Type *new_data) { data = new_data; }
    Type *get_data() { return data; }
public:
    SmartPointer() { set_data(NULL); }
    SmartPointer(Type *data) { set_data(data); }
    SmartPointer(const SmartPointer &other) {
        /* NOTE: this const_cast is ugly. */
        set_data(const_cast<Type *>(other.get_data()));
    }
    
};

} // namespace Misc
} // namespace Aesalon

#endif
