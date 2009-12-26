#include "Types.h"

namespace Aesalon {
namespace Monitor {

Block::Block(Byte *data, std::size_t data_size) {
    this->data.reserve(data_size);
    
    for(std::size_t x = 0; x < data_size; x ++) {
        this->data.push_back(data[x]);
    }
}

void Block::remove(std::size_t from, std::size_t to) {
    if(from > get_size() || to > get_size() || from >= to) return;
    data.erase(data.begin()+from, data.begin()+to);
}

} // namespace Monitor
} // namespace Aesalon
