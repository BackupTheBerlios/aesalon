#include <iostream>
#include "Types.h"




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

Block *Block::subset(std::size_t from, std::size_t to) const {
    if(from > get_size() || to > get_size() || from >= to) return NULL;
    /* NOTE: this is rather inefficient, since the Block constuctor is copying this data again . . . */
    std::vector<Byte> temp_data(data.begin() + from, data.begin() + to);
    return new Block(&temp_data[0], temp_data.size());
}

void Block::read(void *data, std::size_t size) {
    if(size > get_size()) return;
    if(data == NULL) return;
    for(std::size_t x = 0; x < size; x ++) {
        *((Byte *)data + x) = *get_data(x);
    }
    remove(0, size);
}

void Block::hexdump() {
    std::cout << std::hex;
    std::size_t x = 0;
    for(x = 0; x < get_size(); x ++) {
        std::cout << (int)*get_data(x) << " ";
        if((x % 40) == 39) std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}



