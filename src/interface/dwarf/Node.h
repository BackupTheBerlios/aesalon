#ifndef AESALON_INTERFACE_DWARF_NODE_H
#define AESALON_INTERFACE_DWARF_NODE_H

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Node {
private:
    Misc::SmartPointer<Node> child;
    Misc::SmartPointer<Node> next;
    Misc::SmartPointer<Node> prev;
public:
    Node() {}
    virtual ~Node() {}
    
    bool has_children() const { return child.is_valid(); }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
