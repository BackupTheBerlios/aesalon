#include <iostream>

#include "TreeHead.h"

uint64_t TreeHead::m_headValue = 0x8000000000000000ull;

TreeHead::TreeHead(uint32_t headID) : m_headID(headID), m_headNode(NULL) {
	
}

TreeHead::~TreeHead() {
	
}

void TreeHead::attachTo(TreeHead *tree) {
	m_headNode = new TreeNode();
	m_headNode->setHeadID(m_headID);
	/*m_headNode->setValue(m_headValue);*/
	if(tree->headNode()) {
		m_headNode->setLeft(tree->headNode()->left());
		m_headNode->setRight(tree->headNode()->right());
	}
}

void TreeHead::addEvent(Event *event) {
	m_eventList.push_back(event);
}

TreeNode *TreeHead::lookup(uint64_t address) {
	return lookup(address, 0);
}

TreeNode *TreeHead::create(uint64_t address) {
	return lookup(address, CREATE);
}

void TreeHead::remove(uint64_t address) {
	lookup(address, REMOVE);
}

TreeNode *TreeHead::lookup(uint64_t address, int lookupMode) {
	bool create = lookupMode & CREATE;
	if(m_headNode == NULL && create == false) return NULL;
	else if(m_headNode == NULL && create == true) {
		m_headNode = new TreeNode();
		m_headNode->setHeadID(m_headID);
	}
	
	TreeNode *node = m_headNode;
	
	for(int8_t bit = 63; bit >= 0; bit --) {
		uint64_t mask = (uint64_t(0x01) << bit);
		int value = (address & mask)?1:0;
		TreeNode *child = NULL;
		if(value == 1) {
			child = node->right();
		}
		else {
			child = node->left();
		}
		
		if(child == NULL && !create) {
			return NULL;
		}
		else if(child == NULL) {
			child = new TreeNode();
			child->setHeadID(m_headID);
			if(value == 1) node->setRight(child);
			else node->setLeft(child);
		}
		
		node = child;
	}
	
	return node;
}

void TreeHead::mark(uint64_t address) {
	if(m_headNode == NULL) {
		m_headNode = new TreeNode();
		m_headNode->setHeadID(m_headID);
	}
	
	TreeNode *node = m_headNode;
	
	for(int8_t bit = 63; bit >= 0; bit --) {
		uint64_t mask = (uint64_t(0x01) << bit);
		int value = (address & mask)?1:0;
		TreeNode *child = NULL;
		if(value == 1) {
			child = node->right();
		}
		else {
			child = node->left();
		}
		
		if(child == NULL) {
			std::cout << "Trying to mark nonexistent node as changed." << std::endl;
			break;
		}
		else if(child->headID() != m_headID) {
			TreeNode *node = new TreeNode();
			node->setHeadID(m_headID);
			node->setLeft(child->left());
			node->setRight(child->right());
			child = node;
		}
		
		node = child;
	}
}
