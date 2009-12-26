#ifndef AESALON_MONITOR_TYPES_H
#define AESALON_MONITOR_TYPES_H

#include <vector>

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {

typedef unsigned char Byte;
typedef long SWord;
typedef unsigned long Word;

class Block {
    Byte *data;
    std::size_t data_size;
public:
    Block() : data(data), data_size(0) {}
    Block(Byte *data, std::size_t data_size) : data(data), data_size(data_size) {}
    Byte *get_data() const { return data; }
    void set_data(Byte *new_data) { data = new_data; }
    std::size_t get_size() const { return data_size; }
    void set_size(std::size_t new_size) { data_size = new_size; }
    void remove(std::size_t from, std::size_t to);
    
    Byte *operator[](std::size_t index) const {
        if(index > get_size()) return NULL;
        return get_data() + index;
    }
};

} // namespace Monitor
} // namespace Aesalon

#endif
