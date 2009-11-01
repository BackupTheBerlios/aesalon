#include "TupleElement.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

void TupleElement::add_tuple_item(ParseElementWrapper new_tuple_item) {
    if(!get_tuple_head().is_valid()) {
        set_tuple_head(new_tuple_item);
        new_tuple_item->set_next(NULL);
        new_tuple_item->set_prev(new_tuple_item);
        return;
    }
    get_tuple_head()->get_prev()->set_next(new_tuple_item);
    new_tuple_item->set_prev(get_tuple_head()->get_prev());
    get_tuple_head()->set_prev(new_tuple_item);
    new_tuple_item->set_parent(this);
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
