#ifndef AESALON_SMART_POINTER_H
#define AESALON_SMART_POINTER_H

#include <cstddef>

#include "NullPointerException.h"
#include "InvalidCastException.h"
#include "ReferenceCounter.h"

namespace Aesalon {
namespace Misc {

/** Smart pointer class. Used in conjunction with Misc::ReferenceCounter for
memory-leak detection. */
template<typename Type>
class SmartPointer {
private:
    /** The data that the smart pointer references. */
    Type *data;
    /** Set the data to reference.
        @param new_data The new data for the smart pointer to reference.
    */
    void set_data(Type *new_data) {
        ReferenceCounter::lock_mutex();
        ReferenceCounter::get_instance()->decrement_block<Type>(data);
        data = new_data;
        ReferenceCounter::get_instance()->increment_block<Type>(data);
        ReferenceCounter::unlock_mutex();
    }
    /** Get the data that the pointer references.
        @return The data that the pointer references.
    */
    Type *get_data() const { return data; }
    /** Get the data that the pointer references, checking for a NULL
            reference first.
        @throw NullPointerException If the data is NULL.
        @return The data that the pointer references, guarenteed to be
            non-null.
    */
    Type *get_nonnull_data() const {
        if(data) return data;
        throw NullPointerException("Attempt to dereference NULL smart pointer");
    }
public:
    /** Default constructor. Sets the reference data to NULL. */
    SmartPointer() { data = NULL; }
    
    /** Constructor that takes a pointer data.
        @param data The pointer that the smart pointer should reference.
    */
    SmartPointer(Type *data) { this->data = NULL; set_data(data); }
    
    /** Copy constructor, simply copies the data content over.
        @param other The other SmartPointer to copy.
    */
    SmartPointer(const SmartPointer &other) {
        data = NULL;
        /* NOTE: this const_cast is ugly. */
        set_data(const_cast<Type *>(other.get_data()));
    }
    
    /** Arrow operator.
        @return The data that the smart pointer references, in pointer form.
    */
    Type *operator->() const {
        return get_nonnull_data();
    }
    
    /** Dereference operator.
        @return The data that the smart pointer references, as a reference.
    */
    Type &operator*() const {
        return *get_nonnull_data();
    }
    
    /** Assignment operator. Takes another SmartPointer as an argument.
        @param other Another SmartPointer to set the data reference to.
        @return *this, for equations like while((v = peek_element())) {}
    */
    SmartPointer<Type> operator=(const SmartPointer<Type> &other) {
        /* NOTE: this const_cast is ugly as well. */
        set_data(const_cast<Type *>(other.get_data()));
        return *this;
    }
    
    /** Assignement operator that takes a pointer as an argument.
        @param data The data to reference. Note that this implementation of a
            smart pointer assumes ownership of any raw pointers passed via
            assignment operators or constructors.
    */
    SmartPointer<Type> operator=(Type *data) {
        set_data(data);
        return *this;
    }
    
    /** Comparison operator; takes another smart pointer to compare against.
        @param other The smart pointer to compare pointer data against.
        @return True if the pointers are equal, false otherwise.
    */
    bool operator==(const SmartPointer<Type> &other) const {
        return get_data() == other.get_data();
    }
    
    /** Inverse comparison operator, takes another smart pointer to compare against.
        @param other The smart pointer to compare against.
        @return True if the pointers reference different data.
    */
    bool operator!=(const SmartPointer<Type> &other) const {
        return get_data() != other.get_data();
    }
    
    /** Less-than comparison operator; takes a smart pointer to compare
            against.
        @param other A smart pointer, used for comparison.
        @return The result of the pointer comparison.
    */
    bool operator<(const SmartPointer<Type> &other) const {
        return get_data() < other.get_data();
    }
    
    /** Passthrough operator to allow the usage of a SmartPointer in a
            function that takes a Type*.
        @return The data that the smart pointer references.
    */
    operator Type*() {
        return get_data();
    }
    
    /** Passthrough operator to allow usage of a smart pointer where the
        argument is a Type.
        @return The data the smart pointer references, dereferenced.
    */
    operator Type() {
        return *get_nonnull_data();
    }
    operator const Type() const {
        return *get_nonnull_data();
    }
    
    /** Checks if the current smart pointer is referencing a valid object.
        @return True if the pointer is valid.
    */
    bool is_valid() const {
        return get_data() != NULL;
    }
    
    /** Converts the current data to another type.
        @throw InvalidCastException If the types are not polymorphic
            (uses dynamic_cast) or are otherwise incompatible.
        @return A SmartPointer referencing the converted data.
    */
    template<typename ConvertedType>
    SmartPointer<ConvertedType>to() const {
        ConvertedType *ct = dynamic_cast<ConvertedType *>(get_nonnull_data());
        if(ct == NULL) throw InvalidCastException();
        return ct;
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
