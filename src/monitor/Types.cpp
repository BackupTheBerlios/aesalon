#include "Types.h"

namespace Aesalon {
namespace Monitor {

void Block::remove(std::size_t from, std::size_t to) {
    if(from > get_size() || to > get_size() || from >= to) return;
    std::size_t remove_size = to-from;
    
    Byte *old_data = get_data();
    Byte *new_data = new Byte[get_size()-remove_size];
    for(std::size_t x = 0; x < from; x ++) new_data[x] = old_data[x];
    for(std::size_t x = from; (x+remove_size) < get_size(); x ++) new_data[x] = old_data[x + remove_size];
    
    set_size(get_size() - remove_size);
}

} // namespace Monitor
} // namespace Aesalon
