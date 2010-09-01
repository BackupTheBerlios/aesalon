#include "FreeEvent.h"
#include "TreeNode.h"
#include "TreeHead.h"

FreeEvent::FreeEvent(uint64_t address, uint64_t timestamp) : m_address(address), m_timestamp(timestamp) {
	
}

FreeEvent::~FreeEvent() {
	
}

void FreeEvent::applyTo(TreeHead *head) {
	TreeNode *node = head->lookup(m_address);
	
	node->data()->setReleaseTime(m_timestamp);
	head->remove(m_address);
}
