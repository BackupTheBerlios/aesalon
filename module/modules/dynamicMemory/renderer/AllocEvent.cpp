#include <iostream>

#include "AllocEvent.h"
#include "Block.h"
#include "TreeHead.h"
#include "EventVisitor.h"

AllocEvent::AllocEvent(uint64_t address, uint64_t size, uint64_t timestamp)
	: m_address(address), m_size(size), m_timestamp(timestamp) {

}


AllocEvent::~AllocEvent() {
	
}

void AllocEvent::accept(EventVisitor *visitor) {
	visitor->visit(this);
}

void AllocEvent::applyTo(TreeHead *head) {
	TreeNode *node = head->create(m_address);
	if(!node->data()) {
		Block *block = new Block(m_address, m_size, m_timestamp);
		node->setData(block);
	}
	else {
		std::cout << "Trying to apply AllocEvent to tree with block already present." << std::endl;
	}
}
