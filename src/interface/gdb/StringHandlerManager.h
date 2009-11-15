#ifndef AESALON_INTERFACE_GDB_STRING_HANDLER_MANAGER_H
#define AESALON_INTERFACE_GDB_STRING_HANDLER_MANAGER_H

#include <vector>

#include "StringHandler.h"
#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

template<typename StringHandlerType>
class StringHandlerManager {
public:
    typedef std::vector<Misc::SmartPointer<StringHandler> > string_handler_list_t;
private:
    string_handler_list_t string_handler_list;
public:
    StringHandlerManager() {}
    virtual ~StringHandlerManager() {}
    
    void add_string_handler(Misc::SmartPointer<StringHandlerType> handler) {
        string_handler_list.push_back(handler);
    }
    void handle(Misc::SmartPointer<String> string) const {
        if(string.to<StringHandlerType>() == 0) return;
        string_handler_list_t::const_iterator i = string_handler_list.begin();
        for(; i != string_handler_list.end(); i ++) {
            if((*i)->handle(string)) return;
        }
        /* Nothing was able to handle it . . . oh well. */
        
    }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
