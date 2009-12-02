#ifndef AESALON_INTERFACE_GDB_STRING_FOLLOWER_H
#define AESALON_INTERFACE_GDB_STRING_FOLLOWER_H

#include <string>

#include "misc/Exception.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringFollowerException : public Misc::Exception {
public:
    StringFollowerException(std::string message) : Misc::Exception(message) {}
    virtual ~StringFollowerException() {}
};

class StringFollowerInvalidPathException : public StringFollowerException {
public:
    StringFollowerInvalidPathException(std::string message) : StringFollowerException(message) {}
    virtual ~StringFollowerInvalidPathException() {}
};

/** Class to find an element within a String. */
class StringFollower {
private:
    Misc::SmartPointer<ParseData> data;
public:
    StringFollower(Misc::SmartPointer<String> string) : data(string->get_data()) {}
    StringFollower(Misc::SmartPointer<AsyncOutput> async) : data(async->get_data()) {}
    StringFollower(Misc::SmartPointer<ResultRecord> result) : data(result->get_data()) {}
    virtual ~StringFollower() {}
    
    /** Syntax used:
        [n]         nth item in ParseList
        'string'    'string' index in tuple
        "string"    "string" index in tuple
        lhs         Left side of a result (e.g. 'reason' in "reason=breakpoint-hit")
        rhs         Right side of a result (e.g. 'breakpoint-hit' in "reason=breakpoint-hit")
    */
    virtual std::string follow(std::string path);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
