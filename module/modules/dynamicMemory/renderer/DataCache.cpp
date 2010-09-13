#include <iostream>
#include <string.h>
#include <algorithm>

#include "DataCache.h"

DynamicMemoryDataCache::DynamicMemoryDataCache() {
	m_treeHeadVector.push_back(new TreeHead(0));
	std::cout << "m_treeHeadVector.size(): " << m_treeHeadVector.size() << std::endl;
}

DynamicMemoryDataCache::~DynamicMemoryDataCache() {
	
}

void DynamicMemoryDataCache::processPacket(DataPacket *packet) {
	uint8_t type = *(uint8_t *)packet->data;
	
	if(type == 0) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		uint64_t nmemb, size;
		memcpy(&nmemb, (uint8_t *)packet->data + 9, sizeof(nmemb));
		memcpy(&size, (uint8_t *)packet->data + 17, sizeof(size));
		allocBlock(address, nmemb * size, packet->dataSource.timestamp);
	}
	else if(type == 1) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		uint64_t size;
		memcpy(&size, (uint8_t *)packet->data + 9, sizeof(size));
		
		/*std::cout << "malloc(" << size << "): 0x" << std::hex << address << std::dec << std::endl;*/
		allocBlock(address, size, packet->dataSource.timestamp);
	}
	else if(type == 2) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		/*std::cout << "free(0x" << std::hex << address << std::dec << ")" << std::endl;*/
		
		freeBlock(address, packet->dataSource.timestamp);
	}
	else if(type == 3) {
		/* TODO: implement this. */
		std::cout << "realloc currently NYI." << std::endl;
	}
	
	/* TODO: softcode this limit. */
	if(latestTree()->changedList().size() > 500) {
		std::cout << ">500 events in tree, creating new head . . ." << std::endl;
		newHead();
	}
}

static bool compareHeads(TreeHead *one, TreeHead *two) {
	return one->timestamp() < two->timestamp();
}

static bool compareBlocks(Block *one, Block *two) {
	return one->allocTime() < two->allocTime();
}

void DynamicMemoryDataCache::visit(const DataRange &dataRange, CacheVisitor *visitor) {
	TreeHead th(-1);
	th.updateTimestamp(dataRange.beginTime());
	TreeHeadVector::const_iterator startHead = std::lower_bound(m_treeHeadVector.begin(), m_treeHeadVector.end(), &th, compareHeads);
	
	if(startHead == m_treeHeadVector.end()) {
		/* The range is out of range. */
		return;
	}
	
	th.updateTimestamp(dataRange.endTime());
	TreeHeadVector::const_iterator endHead = std::upper_bound(m_treeHeadVector.begin(), m_treeHeadVector.end(), &th, compareHeads);
	
	class RecursiveTraversal {
	public:
		CacheVisitor *m_visitor;
		Timestamp m_lowerCutoff;
		Timestamp m_upperCutoff;
		
		void traverse(TreeNode *node) {
			if(node == NULL) return;
			
			if(node->left()) traverse(node->left());
			if(node->right()) traverse(node->right());
			if(node->data()) {
				Block *block = node->data();
				if(!(block->allocTime() > m_upperCutoff
					|| block->releaseTime() < m_lowerCutoff)) m_visitor->handleBlock(node->data());
			}
		}
	} traverser;
	
	traverser.m_lowerCutoff = dataRange.beginTime();
	traverser.m_upperCutoff = dataRange.endTime();
	traverser.m_visitor = visitor;
	
	traverser.traverse((*startHead)->headNode());
	
	Block block(0, 0, dataRange.beginTime());
	
	TreeHead::BlockList::const_iterator start = std::lower_bound((*startHead)->changedList().begin(), (*startHead)->changedList().end(), &block, compareBlocks);
	
	for(; start != (*startHead)->changedList().end(); start ++) {
		visitor->handleBlock((*start));
	}
	
	if(startHead == endHead) return;
	
	++ startHead;
	
	for(; startHead != endHead; startHead ++) {
		for(TreeHead::BlockList::const_iterator i = (*startHead)->changedList().begin();
			i != (*startHead)->changedList().end() && (*i)->allocTime() < dataRange.endTime(); i ++) {
			
			visitor->handleBlock(*i);
		}
	}
	
}

void DynamicMemoryDataCache::newHead() {
	TreeHead *head = new TreeHead(m_treeHeadVector.size());
	head->attachTo(m_treeHeadVector.back());
	m_treeHeadVector.push_back(head);
}

void DynamicMemoryDataCache::allocBlock(uint64_t address, uint64_t size, Timestamp timestamp) {
	TreeNode *node = latestTree()->create(address);
	
	if(node == NULL) {
		newHead();
		node = latestTree()->create(address);
	}
	
	Block *block = new Block(address, size, timestamp);
	
	node->setData(block);
	latestTree()->appendChanged(block);
	latestTree()->updateTimestamp(timestamp);
}

void DynamicMemoryDataCache::freeBlock(uint64_t address, Timestamp timestamp) {
	TreeNode *node = latestTree()->lookup(address);
	if(node == NULL) return;
	if(!latestTree()->remove(address)) {
		newHead();
		latestTree()->remove(address);
	}
	node->data()->setReleaseTime(timestamp);
	latestTree()->appendChanged(node->data());
	latestTree()->updateTimestamp(timestamp);
}

DataRange DynamicMemoryDataCache::densityRange() const {
	/* TODO: calculate total time and data range. (step through each?) */
	return DataRange(m_treeHeadVector.front()->changedList().front()->allocTime(), m_treeHeadVector.front()->changedList().front()->address(),
		m_treeHeadVector.front()->changedList().front()->releaseTime(), m_treeHeadVector.front()->changedList().front()->address() + m_treeHeadVector.front()->changedList().front()->size() + 128);
}
