#include <iostream>
#include <string.h>

#include "DataCache.h"
#include "Event.h"
#include "AllocEvent.h"
#include "FreeEvent.h"

DynamicMemoryDataCache::DynamicMemoryDataCache() {
	m_treeHeadVector.push_back(new TreeHead(0));
}

DynamicMemoryDataCache::~DynamicMemoryDataCache() {
	
}

void DynamicMemoryDataCache::processPacket(DataPacket *packet) {
	Event *event = NULL;
	
	uint8_t type = *(uint8_t *)packet->data;
	
	if(type == 0) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		uint64_t nmemb, size;
		memcpy(&nmemb, (uint8_t *)packet->data + 9, sizeof(nmemb));
		memcpy(&size, (uint8_t *)packet->data + 17, sizeof(size));
		
		event = new AllocEvent(address, nmemb * size, packet->dataSource.timestamp);
	}
	else if(type == 1) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		uint64_t size;
		memcpy(&size, (uint8_t *)packet->data + 9, sizeof(size));
		
		std::cout << "malloc(" << size << "): 0x" << std::hex << address << std::dec << std::endl;
		
		event = new AllocEvent(address, size, packet->dataSource.timestamp);
	}
	else if(type == 2) {
		uint64_t address;
		memcpy(&address, (uint8_t *)packet->data + 1, sizeof(address));
		
		std::cout << "free(0x" << std::hex << address << std::dec << ")" << std::endl;
		
		event = new FreeEvent(address, packet->dataSource.timestamp);
	}
	else if(type == 3) {
		std::cout << "realloc currently NYI." << std::endl;
	}
	
	if(event != NULL) {
		event->applyTo(m_treeHeadVector.back());
		m_treeHeadVector.back()->addEvent(event);
	}
	
	/* TODO: softcode the event count limit. */
	if(m_treeHeadVector.back()->eventCount() >= 50) newHead();
}

void DynamicMemoryDataCache::visit(EventVisitor *visitor, const DataRange &range) {
	
}

void DynamicMemoryDataCache::newHead() {
	TreeHead *head = new TreeHead(m_treeHeadVector.size());
	head->attachTo(m_treeHeadVector.back());
	m_treeHeadVector.push_back(head);
}
