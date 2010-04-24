#ifndef AESALON_GUI_STORAGE_BACKTRACE_H
#define AESALON_GUI_STORAGE_BACKTRACE_H

#include <QList>

#include "Scope.h"

class Backtrace {
private:
    Scope *scope_list;
    int scope_list_size;
public:
    Backtrace(Scope *scope_list, int scope_list_size);
    
    Scope *get_scope_list() const { return scope_list; }
    int get_scope_list_size() const { return scope_list_size; }
};

#endif
