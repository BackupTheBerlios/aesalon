/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file misc/Singleton.h
*/

#ifndef AESALON_MISC_SINGLETON_H
#define AESALON_MISC_SINGLETON_H

#include "exception/BasicException.h"

namespace Misc {

/** Exception class, thrown when a singleton is initialized for the second time.
*/
class MultipleSingletonException : public Exception::BasicException {
public:
    MultipleSingletonException() : Exception::BasicException("Singleton already initialized") {}
};

/** Exception class, thrown when a singleton instance is requested before the
    singleton has been initialized.
*/
class UnitializedSingletonException : public Exception::BasicException {
public:
    UnitializedSingletonException() : Exception::BasicException("Singleton not initialized") {}
};

/** Singleton class, contains a basic singleton implementation. */
template<typename Type>
class Singleton {
private:
    /** Singleton instance, declared in each singleton derived class. */
    static Type *instance;
public:
    /** Basic constructor, checks if the singleton has been initialized yet.
        @throw MultipleSingletonException If the singleton has already been
            initialized.
    */
    Singleton() {
        if(instance) throw MultipleSingletonException();
        instance = static_cast<Type *>(this);
    }
    
    /** Virtual destructor. If the instance is initialized, set the pointer to
            zero, so that it can be initialized again if nessasary.
    */
    virtual ~Singleton() {
        if(instance) instance = 0;
    }
    
    /** Static member to return the singleton instance.
        @throw UnitializedSingletonException If the instance is not initialized.
        @return The singleton instance, as a pointer.
    */
    static Type *get_instance() {
        if(!instance) throw UnitializedSingletonException();
        return instance;
    }
};

} // namespace Misc

#endif
