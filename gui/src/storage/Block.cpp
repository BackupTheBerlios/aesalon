#include "Block.h"

Block* Block::clone() {
    Block *block = new Block(get_allocation_time(), address, size);
    block->set_release_time(get_release_time());
    return block;
}
