#ifndef AESALON_GUI_SESSION_SCOPE_MAPPER_H
#define AESALON_GUI_SESSION_SCOPE_MAPPER_H

#include <QList>
#include <QString>

#include "storage/Scope.h"

class ScopeMapper {
private:
    QList<Scope> scope_list;
public:
    ScopeMapper();
    virtual ~ScopeMapper();
    
    void add_scope(const Scope &scope);
    const Scope &get_scope(quint32 id);
};

#endif
