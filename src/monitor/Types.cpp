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

Misc::SmartPointer<Block> Block::subset(std::size_t from, std::size_t to) const {
    if(from > get_size() || to > get_size() || from >= to) return NULL;
    /* NOTE: this is rather inefficient, since the Block constuctor is copying this data again . . . */
    std::vector<Byte> temp_data(data.begin() + from, data.begin() + to);
    return new Block(&temp_data[0], temp_data.size());
}

} // namespace Monitor
} // namespace Aesalon
