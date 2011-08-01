/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/storage/Mempool.cpp
*/

#include "storage/Mempool.h"
#include "util/MessageSystem.h"

namespace Storage {

Mempool::ElementSize Mempool::m_elementSizes[AesalonPoolSizeThreshold];

uint64_t *Mempool::m_poolHead;
uint64_t *Mempool::m_poolTail;

void Mempool::create() {
	for(int i = 0; i < AesalonPoolSizeThreshold; i ++) {
		m_elementSizes[i].start = m_elementSizes[i].end = NULL;
	}
	m_poolHead = m_poolTail = NULL;
}

void Mempool::destroy() {
	uint64_t *p = m_poolHead;
	while(p != NULL) {
		uint64_t *next = (uint64_t *)p[0];
		delete[] p;
		p = next;
	}
	
	/* Leave the global variables nice and clean in case we want to create the mempool again. */
	for(int i = 0; i < AesalonPoolSizeThreshold; i ++) {
		m_elementSizes[i].start = m_elementSizes[i].end = NULL;
	}
	m_poolHead = m_poolTail = NULL;
}

void *Mempool::allocate(uint64_t size) {
	size = normalize(size);
	
	int8_t index = logBase2(size);
	
	if(m_elementSizes[index].start == NULL) allocatePool(size);
	
	uint64_t address = *m_elementSizes[index].start;
	m_elementSizes[index].start = (uint64_t *)*((uint64_t *)address);
	
	return (void *)address;
}

void Mempool::release(void *data, uint64_t size) {
	size = normalize(size);
	
	uint64_t *ptr = (uint64_t *)data;
	*ptr = 0;
	
	int8_t index = logBase2(size);
	
	if(m_elementSizes[index].end != NULL) {
		*m_elementSizes[index].end = (uint64_t)ptr;
	}
	m_elementSizes[index].end = ptr;
}

uint64_t Mempool::normalize(uint64_t size) {
	/* This rounds up to the nearest power of two.
		Source: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	*/
	size --;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size |= size >> 32;
	size ++;
	
	/* Minimum pool element size is eight bytes. */
	if(size < 8) size = 8;
	return size;
}

int8_t Mempool::logBase2(uint64_t value) {
	if(value == 0x0) return -1;
	
#define MapLog2(n) \
	else if(value == (1ul << n)) return n
	
	MapLog2(0);
	MapLog2(1);
	MapLog2(2);
	MapLog2(3);
	MapLog2(4);
	MapLog2(5);
	MapLog2(6);
	MapLog2(7);
	MapLog2(8);
	MapLog2(9);
	MapLog2(10);
	MapLog2(11);
	MapLog2(12);
	MapLog2(13);
	MapLog2(14);
	MapLog2(15);
	MapLog2(16);
	MapLog2(17);
	MapLog2(18);
	MapLog2(19);
	MapLog2(20);
	MapLog2(21);
	MapLog2(22);
	MapLog2(23);
	MapLog2(24);
	MapLog2(25);
	MapLog2(26);
	MapLog2(27);
	MapLog2(28);
	MapLog2(29);
	MapLog2(30);
	MapLog2(31);
	MapLog2(32);
	MapLog2(33);
	MapLog2(34);
	MapLog2(35);
	MapLog2(36);
	MapLog2(37);
	MapLog2(38);
	MapLog2(39);
	MapLog2(40);
	MapLog2(41);
	MapLog2(42);
	MapLog2(43);
	MapLog2(44);
	MapLog2(45);
	MapLog2(46);
	MapLog2(47);
	MapLog2(48);
	MapLog2(49);
	MapLog2(50);
	MapLog2(51);
	MapLog2(52);
	MapLog2(53);
	MapLog2(54);
	MapLog2(55);
	MapLog2(56);
	MapLog2(57);
	MapLog2(58);
	MapLog2(59);
	MapLog2(60);
	MapLog2(61);
	MapLog2(62);
	MapLog2(63);
	
#undef MapLog2
	
	Message2(Warning, Storage, "logBase2 passed non-power of two.");
	return -1;
}

void Mempool::allocatePool(uint64_t size) {
	uint64_t *data = new uint64_t[size * AesalonPoolSize];
	data[0] = 0;
	data[1] = size;
	
	if(m_poolHead == NULL) m_poolHead = data;
	
	if(m_poolTail) m_poolTail[0] = (uint64_t)data;
	m_poolTail = data;
	
	/* Get the element size index. */
	int8_t index = logBase2(size);
	
	/* Chain together elements inside this pool. */
	for(int i = 1; i < (AesalonPoolSize-1); i ++) {
		data[i*(size >> 3)] = (uint64_t)&data[(i+1)*(size >> 3)];
	}
	/* The last element doesn't point to anything. */
	data[(AesalonPoolSize-1)*(size >> 3)] = 0;
	
	/* Add the first element in this chain onto any pre-existing chains. */
	if(m_elementSizes[index].end != 0) {
		*m_elementSizes[index].end = (uint64_t)&data[(size >> 3)];
	}
	/* Set the end element to the last element in this chain. */
	m_elementSizes[index].end = &data[(AesalonPoolSize-1)*(size >> 3)];
	
	/* If it is not set, set the first element to the beginning of the element size's chain. */
	if(m_elementSizes[index].start == NULL) {
		m_elementSizes[index].start = &data[(size >> 3)];
	}
}

} // namespace Storage
