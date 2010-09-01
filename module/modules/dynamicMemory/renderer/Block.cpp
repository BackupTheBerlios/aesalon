#include "Block.h"

Block::Block(uint64_t address, uint64_t size, uint64_t allocTime)
	: m_address(address), m_size(size), m_allocTime(allocTime), m_reallocedTo(NULL) {

}
