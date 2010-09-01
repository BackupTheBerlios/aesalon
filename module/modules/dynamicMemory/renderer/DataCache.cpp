#include <iostream>
#include "DataCache.h"

DynamicMemoryDataCache::DynamicMemoryDataCache() {
	m_treeHeadVector.push_back(new TreeHead(0));
}

DynamicMemoryDataCache::~DynamicMemoryDataCache() {
	
}

void DynamicMemoryDataCache::processPacket(DataPacket *packet) {
	static int count = 0;
	count ++;
	m_treeHeadVector.back()->create(count)->setData((Block *)count);
	std::cout << "looked up: " << m_treeHeadVector.back()->lookup(count)->data() << std::endl;
}

void DynamicMemoryDataCache::newHead() {
	TreeHead *head = new TreeHead(m_treeHeadVector.size());
	head->attachTo(m_treeHeadVector.back());
	m_treeHeadVector.push_back(head);
}
