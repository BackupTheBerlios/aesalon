#include "Scope.h"

namespace Tracker {

Scope::Scope(ScopeID id, Word address, std::string name) {
}

Scope::~Scope() {
}


bool Scope::operator<(const Word address) const {
    return this->address < address;
}

bool Scope::operator<=(const Word address) const {
    return this->address <= address;
}

} // namespace Tracker
