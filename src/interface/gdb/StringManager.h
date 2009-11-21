#ifndef AESALON_INTERFACE_GDB_STRING_MANAGER_H
#define AESALON_INTERFACE_GDB_STRING_MANAGER_H

#include <vector>

#include "StringHandler.h"
#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringManager {
public:
    typedef std::vector<Misc::SmartPointer<StringHandler> > string_handler_list_t;
private:
    string_handler_list_t string_handler_list;
    Misc::SmartPointer<Controller> controller;
public:
    StringManager() {}
    virtual ~StringManager() {}
    
    void add_string_handler(Misc::SmartPointer<StringHandler> handler) {
        string_handler_list.push_back(handler);
    }
    void handle(Misc::SmartPointer<String> string) const {
        string_handler_list_t::const_iterator i = string_handler_list.begin();
        for(; i != string_handler_list.end(); i ++) {
            try {
                if((*i)->handle(string)) return;
            }
            catch(Misc::InvalidCastException ice) {}
        }
        /* Nothing was able to handle it . . . oh well. */
    }
    
    Misc::SmartPointer<Controller> get_controller() const { return controller; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
