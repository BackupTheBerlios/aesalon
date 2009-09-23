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
    Type *get_data() const { return data; }
    Type *get_nonnull_data() const {
        if(data) return data;
        throw NullPointerException("dereferencing NULL smart pointer");
    }
public:
    SmartPointer() { set_data(NULL); }
    SmartPointer(Type *data) { set_data(data); }
    SmartPointer(const SmartPointer &other) {
        /* NOTE: this const_cast is ugly. */
        set_data(const_cast<Type *>(other.get_data()));
    }
    
    Type *operator->() {
        return get_nonnull_data();
    }
    
    Type &operator*() {
        return *get_nonnull_data();
    }
    Type *operator&() {
        return get_nonnull_data();
    }
    
    SmartPointer<Type> operator=(const SmartPointer<Type> &other) {
        /* NOTE: this const_cast is ugly as well. */
        set_data(const_cast<Type *>(other.get_data()));
        return *this;
    }
    SmartPointer<Type> operator=(Type *data) {
        set_data(data);
        return *this;
    }
    
    bool operator==(const SmartPointer<Type> &other) const {
        if(get_data() == NULL && other.get_data() == NULL) return true;
        return *get_nonnull_data() == *other.get_nonnull_data();
    }
    
    bool operator<(const SmartPointer<Type> &other) const {
        if(get_data() == NULL && other.get_data() == NULL) return false;
        return *get_nonnull_data() < *other.get_nonnull_data();
    }
    
    operator Type*() {
        return get_data();
    }
    operator Type() {
        return *get_nonnull_data();
    }
    
    bool is_null() {
        return get_data() == NULL;
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
