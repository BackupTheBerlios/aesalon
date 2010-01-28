#ifndef AESALON_MISC_SINGLETON_H
#define AESALON_MISC_SINGLETON_H

#include <pthread.h>

#include "Exception.h"

namespace Aesalon {
namespace Misc {

/** Exception class, thrown when a singleton is initialized for the second
    time.
*/
class MultipleSingletonException : public Exception {
public:
    MultipleSingletonException() : Exception("Singleton already initialized") {}
};

/** Exception class, thrown when a singleton instance is requested before the
    singleton has been initialized.
*/
class UnitializedSingletonException : public Exception {
public:
    UnitializedSingletonException() : Exception("Singleton not initialized") {}
};

/** Singleton class, contains a basic singleton implementation. */
template<typename Type>
class Singleton {
private:
    /** Singleton instance, declared in each singleton derived class. */
    static Type *instance;
    /** Singleton multithreading mutex. Makes the singleton class
        thread-safe. */
    pthread_mutex_t singleton_mutex;
    
    /** Returns the lockable mutex for the current singleton.
        @return A lockable mutex, which helps make the singleton library
            thread-safe.
    */
    pthread_mutex_t *get_mutex() { return &singleton_mutex; }
public:
    /** Basic constructor, checks if the singleton has been initialized yet.
        @throw MultipleSingletonException If the singleton has already been
            initialized.
    */
    Singleton() {
        if(instance) throw MultipleSingletonException();
        instance = static_cast<Type *>(this);
        pthread_mutex_init(&singleton_mutex, NULL);
    }
    
    /** Virtual destructor. If the instance is initialized, set the pointer to
            zero, so that it can be initialized again if nessasary.
    */
    virtual ~Singleton() {
        if(instance) instance = 0;
        pthread_mutex_destroy(&singleton_mutex);
    }
    
    /** Static member to return the singleton instance.
        @throw UnitializedSingletonException If the instance is not initialized.
        @return The singleton instance, as a pointer.
    */
    static Type *get_instance() {
        if(!instance) throw UnitializedSingletonException();
        return instance;
    }
    
    static void lock_mutex() {
        pthread_mutex_lock(get_instance()->get_mutex());
    }
    
    static void unlock_mutex() {
        pthread_mutex_unlock(get_instance()->get_mutex());
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
