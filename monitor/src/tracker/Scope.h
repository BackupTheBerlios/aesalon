#ifndef AESALON_TRACKER_SCOPE_H
#define AESALON_TRACKER_SCOPE_H

#include <string>

#include "Types.h"

namespace Tracker {

typedef Word ScopeID;

class Scope {
private:
    ScopeID id;
    Word address;
    std::string name;
public:
    Scope(ScopeID id, Word address, std::string name);
    ~Scope();
    
    ScopeID get_id() const { return id; }
    Word get_address() const { return address; }
    const std::string &get_name() const { return name; }
    
    bool operator<(const Word address) const;
    bool operator<=(const Word address) const;
};

} // namespace Tracker

#endif
