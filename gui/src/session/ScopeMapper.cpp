#include <QObject>

#include "ScopeMapper.h"

ScopeMapper::ScopeMapper() {
    add_scope(QObject::tr("Unknown"));
}

ScopeMapper::~ScopeMapper() {
    
}

void ScopeMapper::add_scope(const Scope &scope) {
    scope_list.append(scope);
}

const Scope &ScopeMapper::get_scope(quint32 id) {
    return scope_list[id];
}
